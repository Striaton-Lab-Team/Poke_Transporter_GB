#ifndef LINK_SPI_H
#define LINK_SPI_H

// --------------------------------------------------------------------------
// An SPI handler for the Link Port (Normal Mode, 32bits).
// --------------------------------------------------------------------------
// Usage:
// - 1) Include this header in your main.cpp file and add:
//       LinkSPI* linkSPI = new LinkSPI();
// - 2) (Optional) Add the interrupt service routines: (*)
//       irq_init(NULL);
//       irq_add(II_SERIAL, LINK_SPI_ISR_SERIAL);
//       // (this is only required for `transferAsync`)
// - 3) Initialize the library with:
//       linkSPI->activate(LinkSPI::Mode::MASTER_256KBPS);
//       // (use LinkSPI::Mode::SLAVE on the other end)
// - 4) Exchange 32-bit data with the other end:
//       u32 data = linkSPI->transfer(0x12345678);
//       // (this blocks the console indefinitely)
// - 5) Exchange data with a cancellation callback:
//       u32 data = linkSPI->transfer(0x12345678, []() {
//         u16 keys = ~REG_KEYS & KEY_ANY;
//         return keys & KEY_START;
//       });
// - 6) Exchange data asynchronously:
//       linkSPI->transferAsync(0x12345678);
//       // ...
//       if (linkSPI->getAsyncState() == LinkSPI::AsyncState::READY) {
//         u32 data = linkSPI->getAsyncData();
//         // ...
//       }
// --------------------------------------------------------------------------
// (*) libtonc's interrupt handler sometimes ignores interrupts due to a bug.
//     That causes packet loss. You REALLY want to use libugba's instead.
//     (see examples)
// --------------------------------------------------------------------------
// considerations:
// - when using Normal Mode between two GBAs, use a GBC Link Cable!
// - only use the 2Mbps mode with custom hardware (very short wires)!
// - don't send 0xFFFFFFFF, it's reserved for errors!
// --------------------------------------------------------------------------

#include <tonc_core.h>

// 8-bit mode (uncomment to enable)
#define LINK_SPI_8BIT_MODE

#ifdef LINK_SPI_8BIT_MODE
#define LINK_SPI_DATA_TYPE u8
#endif
#ifndef LINK_SPI_8BIT_MODE
#define LINK_SPI_DATA_TYPE u32
#endif

#ifdef LINK_SPI_8BIT_MODE
#define LINK_SPI_DATA_REG REG_SIODATA8
#endif
#ifndef LINK_SPI_8BIT_MODE
#define LINK_SPI_DATA_REG REG_SIODATA32
#endif

#ifdef LINK_SPI_8BIT_MODE
#define LINK_SPI_NO_DATA 0xff
#endif
#ifndef LINK_SPI_8BIT_MODE
#define LINK_SPI_NO_DATA 0xffffffff
#endif

#define LINK_SPI_BIT_CLOCK 0
#define LINK_SPI_BIT_CLOCK_SPEED 1
#define LINK_SPI_BIT_SI 2
#define LINK_SPI_BIT_SO 3
#define LINK_SPI_BIT_START 7
#define LINK_SPI_BIT_LENGTH 12
#define LINK_SPI_BIT_IRQ 14
#define LINK_SPI_BIT_GENERAL_PURPOSE_LOW 14
#define LINK_SPI_BIT_GENERAL_PURPOSE_HIGH 15

static volatile char LINK_SPI_VERSION[] = "LinkSPI/v6.2.1";

const u32 LINK_SPI_MASK_CLEAR_SO_BIT = ~(1 << LINK_SPI_BIT_SO);
const u32 LINK_SPI_MASK_SET_START_BIT = (1 << LINK_SPI_BIT_START);

class LinkSPI {
 public:
  enum Mode { SLAVE, MASTER_256KBPS, MASTER_2MBPS };
  enum AsyncState { IDLE, WAITING, READY };

  bool isActive() { return isEnabled; }

  void activate(Mode mode) {
    this->mode = mode;
    this->waitMode = false;
    this->asyncState = IDLE;
    this->asyncData = 0;

    setNormalMode32Bit();
    disableTransfer();

    if (mode == SLAVE)
      setSlaveMode();
    else {
      setMasterMode();

      if (mode == MASTER_256KBPS)
        set256KbpsSpeed();
      else if (mode == MASTER_2MBPS)
        set2MbpsSpeed();
    }

    isEnabled = true;
  }

  void deactivate() {
    isEnabled = false;
    setGeneralPurposeMode();

    mode = SLAVE;
    waitMode = false;
    asyncState = IDLE;
    asyncData = 0;
  }

  LINK_SPI_DATA_TYPE transfer(LINK_SPI_DATA_TYPE data) {
    return transfer(data, []() { return false; });
  }

  template <typename F>
  LINK_SPI_DATA_TYPE transfer(LINK_SPI_DATA_TYPE data,
                              F cancel,
                              bool _async = false,
                              bool _customAck = false) {
    if (asyncState != IDLE)
      return LINK_SPI_NO_DATA;

    setData(data);

    if (_async) {
      asyncState = WAITING;
      setInterruptsOn();
    } else {
      setInterruptsOff();
    }

    while (isMaster() && waitMode && !isSlaveReady())
      if (cancel()) {
        disableTransfer();
        setInterruptsOff();
        asyncState = IDLE;
        return LINK_SPI_NO_DATA;
      }

    enableTransfer();
    startTransfer();

    if (_async)
      return LINK_SPI_NO_DATA;

    while (!isReady())
      if (cancel()) {
        stopTransfer();
        disableTransfer();
        return LINK_SPI_NO_DATA;
      }

    if (!_customAck)
      disableTransfer();

    return getData();
  }

  void transferAsync(LINK_SPI_DATA_TYPE data) {
    transfer(
        data, []() { return false; }, true);
  }

  template <typename F>
  void transferAsync(LINK_SPI_DATA_TYPE data, F cancel) {
    transfer(data, cancel, true);
  }

  LINK_SPI_DATA_TYPE getAsyncData() {
    if (asyncState != READY)
      return LINK_SPI_NO_DATA;

    LINK_SPI_DATA_TYPE data = asyncData;
    asyncState = IDLE;
    return data;
  }

  Mode getMode() { return mode; }
  void setWaitModeActive(bool isActive) { waitMode = isActive; }
  bool isWaitModeActive() { return waitMode; }
  AsyncState getAsyncState() { return asyncState; }

  void _onSerial(bool _customAck = false) {
    if (!isEnabled || asyncState != WAITING)
      return;

    if (!_customAck)
      disableTransfer();

    setInterruptsOff();
    asyncState = READY;
    asyncData = getData();
  }

  void _setSOHigh() { setBitHigh(LINK_SPI_BIT_SO); }
  void _setSOLow() { setBitLow(LINK_SPI_BIT_SO); }
  bool _isSIHigh() { return isBitHigh(LINK_SPI_BIT_SI); }

 private:
  Mode mode = Mode::SLAVE;
  bool waitMode = false;
  AsyncState asyncState = IDLE;
  LINK_SPI_DATA_TYPE asyncData = 0;
  volatile bool isEnabled = false;

  void setNormalMode32Bit() {
    REG_RCNT = REG_RCNT & ~(1 << LINK_SPI_BIT_GENERAL_PURPOSE_HIGH);
#ifdef LINK_SPI_8BIT_MODE
    REG_SIOCNT = 0;
#endif
#ifndef LINK_SPI_8BIT_MODE
    REG_SIOCNT = 1 << LINK_SPI_BIT_LENGTH;
#endif
  }

  void setGeneralPurposeMode() {
    REG_RCNT = (REG_RCNT & ~(1 << LINK_SPI_BIT_GENERAL_PURPOSE_LOW)) |
               (1 << LINK_SPI_BIT_GENERAL_PURPOSE_HIGH);
  }

  void setData(LINK_SPI_DATA_TYPE data) { LINK_SPI_DATA_REG = data; }
  LINK_SPI_DATA_TYPE getData() { return LINK_SPI_DATA_REG; }

  void enableTransfer() { _setSOLow(); }
  void disableTransfer() { _setSOHigh(); }
  void startTransfer() { setBitHigh(LINK_SPI_BIT_START); }
  void stopTransfer() { setBitLow(LINK_SPI_BIT_START); }
  bool isReady() { return !isBitHigh(LINK_SPI_BIT_START); }
  bool isSlaveReady() { return !_isSIHigh(); }

  void setMasterMode() { setBitHigh(LINK_SPI_BIT_CLOCK); }
  void setSlaveMode() { setBitLow(LINK_SPI_BIT_CLOCK); }
  void set256KbpsSpeed() { setBitLow(LINK_SPI_BIT_CLOCK_SPEED); }
  void set2MbpsSpeed() { setBitHigh(LINK_SPI_BIT_CLOCK_SPEED); }
  void setInterruptsOn() { setBitHigh(LINK_SPI_BIT_IRQ); }
  void setInterruptsOff() { setBitLow(LINK_SPI_BIT_IRQ); }

  bool isMaster() { return mode != SLAVE; }
  bool isBitHigh(u8 bit) { return (REG_SIOCNT >> bit) & 1; }
  void setBitHigh(u8 bit) { REG_SIOCNT |= 1 << bit; }
  void setBitLow(u8 bit) { REG_SIOCNT &= ~(1 << bit); }
};

extern LinkSPI* linkSPI;

inline void LINK_SPI_ISR_SERIAL() {
  linkSPI->_onSerial();
}

#endif  // LINK_SPI_H
