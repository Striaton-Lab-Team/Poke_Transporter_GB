#ifndef LINK_RAW_CABLE_H
#define LINK_RAW_CABLE_H

// --------------------------------------------------------------------------
// A low level handler for the Link Port (Multi-Play Mode).
// --------------------------------------------------------------------------
// Usage:
// - 1) Include this header in your main.cpp file and add:
//       LinkRawCable* linkRawCable = new LinkRawCable();
// - 2) (Optional) Add the interrupt service routines:
//       irq_init(NULL);
//       irq_add(II_SERIAL, LINK_RAW_CABLE_ISR_SERIAL);
//       // (this is only required for `transferAsync`)
// - 3) Initialize the library with:
//       linkRawCable->activate();
// - 4) Exchange 16-bit data with the connected consoles:
//       LinkRawCable::Response data = linkRawCable->transfer(0x1234);
//       // (this blocks the console indefinitely)
// - 5) Exchange data with a cancellation callback:
//       u16 data = linkRawCable->transfer(0x1234, []() {
//         u16 keys = ~REG_KEYS & KEY_ANY;
//         return keys & KEY_START;
//       });
// - 6) Exchange data asynchronously:
//       linkRawCable->transferAsync(0x1234);
//       // ...
//       if (linkRawCable->getAsyncState() == LinkRawCable::AsyncState::READY) {
//         u16 data = linkRawCable->getAsyncData();
//         // ...
//       }
// --------------------------------------------------------------------------
// considerations:
// - don't send 0xFFFF, it's a reserved value that means <disconnected client>
// - only transfer(...) if isReady()
// --------------------------------------------------------------------------

#include <tonc_core.h>

#define LINK_RAW_CABLE_MAX_PLAYERS 4
#define LINK_RAW_CABLE_DISCONNECTED 0xffff
#define LINK_RAW_CABLE_BIT_SLAVE 2
#define LINK_RAW_CABLE_BIT_READY 3
#define LINK_RAW_CABLE_BITS_PLAYER_ID 4
#define LINK_RAW_CABLE_BIT_ERROR 6
#define LINK_RAW_CABLE_BIT_START 7
#define LINK_RAW_CABLE_BIT_MULTIPLAYER 13
#define LINK_RAW_CABLE_BIT_IRQ 14
#define LINK_RAW_CABLE_BIT_GENERAL_PURPOSE_LOW 14
#define LINK_RAW_CABLE_BIT_GENERAL_PURPOSE_HIGH 15
#define LINK_RAW_CABLE_EMPTY_RESPONSE                              \
  {                                                                \
    {                                                              \
      LINK_RAW_CABLE_DISCONNECTED, LINK_RAW_CABLE_DISCONNECTED,    \
          LINK_RAW_CABLE_DISCONNECTED, LINK_RAW_CABLE_DISCONNECTED \
    }                                                              \
  }

static volatile char LINK_RAW_CABLE_VERSION[] = "LinkRawCable/v6.2.1";

class LinkRawCable {
 public:
  enum BaudRate {
    BAUD_RATE_0,  // 9600 bps
    BAUD_RATE_1,  // 38400 bps
    BAUD_RATE_2,  // 57600 bps
    BAUD_RATE_3   // 115200 bps
  };
  struct Response {
    u16 data[LINK_RAW_CABLE_MAX_PLAYERS];
    int playerId = -1;  // (-1 = unknown)
  };
  enum AsyncState { IDLE, WAITING, READY };

  bool isActive() { return isEnabled; }

  void activate(BaudRate baudRate = BaudRate::BAUD_RATE_1) {
    this->baudRate = baudRate;
    this->asyncState = IDLE;
    this->asyncData = LINK_RAW_CABLE_EMPTY_RESPONSE;

    setMultiPlayMode();
    isEnabled = true;
  }

  void deactivate() {
    isEnabled = false;
    setGeneralPurposeMode();

    baudRate = BaudRate::BAUD_RATE_1;
    asyncState = IDLE;
    asyncData = LINK_RAW_CABLE_EMPTY_RESPONSE;
  }

  Response transfer(u16 data) {
    return transfer(data, []() { return false; });
  }

  template <typename F>
  Response transfer(u16 data, F cancel, bool _async = false) {
    if (asyncState != IDLE)
      return LINK_RAW_CABLE_EMPTY_RESPONSE;

    setData(data);

    if (_async) {
      asyncState = WAITING;
      setInterruptsOn();
    } else {
      setInterruptsOff();
    }

    startTransfer();

    if (_async)
      return LINK_RAW_CABLE_EMPTY_RESPONSE;

    while (isSending())
      if (cancel()) {
        stopTransfer();
        return LINK_RAW_CABLE_EMPTY_RESPONSE;
      }

    if (isReady() && !hasError())
      return getData();

    return LINK_RAW_CABLE_EMPTY_RESPONSE;
  }

  void transferAsync(u16 data) {
    transfer(
        data, []() { return false; }, true);
  }

  Response getAsyncData() {
    if (asyncState != READY)
      return LINK_RAW_CABLE_EMPTY_RESPONSE;

    Response data = asyncData;
    asyncState = IDLE;
    return data;
  }

  BaudRate getBaudRate() { return baudRate; }
  bool isMaster() { return !isBitHigh(LINK_RAW_CABLE_BIT_SLAVE); }
  bool isReady() { return isBitHigh(LINK_RAW_CABLE_BIT_READY); }
  AsyncState getAsyncState() { return asyncState; }

  void _onSerial() {
    if (!isEnabled || asyncState != WAITING)
      return;

    setInterruptsOff();
    asyncState = READY;
    asyncData = LINK_RAW_CABLE_EMPTY_RESPONSE;
    if (isReady() && !hasError())
      asyncData = getData();
  }

 private:
  BaudRate baudRate = BaudRate::BAUD_RATE_1;
  AsyncState asyncState = IDLE;
  Response asyncData = LINK_RAW_CABLE_EMPTY_RESPONSE;
  volatile bool isEnabled = false;

  void setMultiPlayMode() {
    REG_RCNT = REG_RCNT & ~(1 << LINK_RAW_CABLE_BIT_GENERAL_PURPOSE_HIGH);
    REG_SIOCNT = (1 << LINK_RAW_CABLE_BIT_MULTIPLAYER);
    REG_SIOCNT |= baudRate;
    REG_SIOMLT_SEND = 0;
  }

  void setGeneralPurposeMode() {
    REG_RCNT = (REG_RCNT & ~(1 << LINK_RAW_CABLE_BIT_GENERAL_PURPOSE_LOW)) |
               (1 << LINK_RAW_CABLE_BIT_GENERAL_PURPOSE_HIGH);
  }

  void setData(u16 data) { REG_SIOMLT_SEND = data; }
  Response getData() {
    Response response = LINK_RAW_CABLE_EMPTY_RESPONSE;

    for (u32 i = 0; i < LINK_RAW_CABLE_MAX_PLAYERS; i++)
      response.data[i] = REG_SIOMULTI[i];

    response.playerId =
        (REG_SIOCNT & (0b11 << LINK_RAW_CABLE_BITS_PLAYER_ID)) >>
        LINK_RAW_CABLE_BITS_PLAYER_ID;

    return response;
  }

  bool hasError() { return isBitHigh(LINK_RAW_CABLE_BIT_ERROR); }
  bool isSending() { return isBitHigh(LINK_RAW_CABLE_BIT_START); }

  void startTransfer() { setBitHigh(LINK_RAW_CABLE_BIT_START); }
  void stopTransfer() { setBitLow(LINK_RAW_CABLE_BIT_START); }

  void setInterruptsOn() { setBitHigh(LINK_RAW_CABLE_BIT_IRQ); }
  void setInterruptsOff() { setBitLow(LINK_RAW_CABLE_BIT_IRQ); }

  bool isBitHigh(u8 bit) { return (REG_SIOCNT >> bit) & 1; }
  void setBitHigh(u8 bit) { REG_SIOCNT |= 1 << bit; }
  void setBitLow(u8 bit) { REG_SIOCNT &= ~(1 << bit); }
};

extern LinkRawCable* linkRawCable;

inline void LINK_RAW_CABLE_ISR_SERIAL() {
  linkRawCable->_onSerial();
}

#endif  // LINK_RAW_CABLE_H
