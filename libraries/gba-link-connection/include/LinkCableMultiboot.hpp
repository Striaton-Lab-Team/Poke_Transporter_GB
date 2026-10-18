#ifndef LINK_CABLE_MULTIBOOT_H
#define LINK_CABLE_MULTIBOOT_H

// --------------------------------------------------------------------------
// A Multiboot tool to send small programs from one GBA to up to 3 slaves.
// --------------------------------------------------------------------------
// Usage:
// - 1) Include this header in your main.cpp file and add:
//       LinkCableMultiboot* linkCableMultiboot = new LinkCableMultiboot();
// - 2) Send the ROM:
//       LinkCableMultiboot::Result result = linkCableMultiboot->sendRom(
//         romBytes, // for current ROM, use: ((const u8*)MEM_EWRAM)
//         romLength, // in bytes, should be multiple of 0x10
//         []() {
//           u16 keys = ~REG_KEYS & KEY_ANY;
//           return keys & KEY_START;
//           // (when this returns true, the transfer will be canceled)
//         }
//       );
//       // `result` should be LinkCableMultiboot::Result::SUCCESS
// --------------------------------------------------------------------------
// considerations:
// - for better results, turn on the GBAs after calling the `sendRom` method!
// --------------------------------------------------------------------------

#include <tonc_bios.h>
#include <tonc_core.h>
#include "LinkRawCable.hpp"

#define LINK_CABLE_MULTIBOOT_MIN_ROM_SIZE (0x100 + 0xc0)
#define LINK_CABLE_MULTIBOOT_MAX_ROM_SIZE (256 * 1024)
#define LINK_CABLE_MULTIBOOT_WAIT_BEFORE_RETRY ((160 + 68) * 60)
#define LINK_CABLE_MULTIBOOT_WAIT_BEFORE_TRANSFER 50
#define LINK_CABLE_MULTIBOOT_DETECTION_TRIES 16
#define LINK_CABLE_MULTIBOOT_PALETTE_DATA 0x93
#define LINK_CABLE_MULTIBOOT_CLIENTS 3
#define LINK_CABLE_MULTIBOOT_CLIENT_NO_DATA 0xff
#define LINK_CABLE_MULTIBOOT_HANDSHAKE 0x6200
#define LINK_CABLE_MULTIBOOT_HANDSHAKE_RESPONSE 0x7200
#define LINK_CABLE_MULTIBOOT_CONFIRM_CLIENTS 0x6100
#define LINK_CABLE_MULTIBOOT_SEND_PALETTE 0x6300
#define LINK_CABLE_MULTIBOOT_HANDSHAKE_DATA 0x11
#define LINK_CABLE_MULTIBOOT_CONFIRM_HANDSHAKE_DATA 0x6400
#define LINK_CABLE_MULTIBOOT_ACK_RESPONSE 0x73
#define LINK_CABLE_MULTIBOOT_HEADER_SIZE 0xC0
#define LINK_CABLE_MULTIBOOT_SWI_MULTIPLAYER_MODE 1
#define LINK_CABLE_MULTIBOOT_MAX_BAUD_RATE LinkRawCable::BaudRate::BAUD_RATE_3
#define LINK_CABLE_MULTIBOOT_TRY(CALL)    \
  do {                                    \
    partialResult = CALL;                 \
  } while (partialResult == NEEDS_RETRY); \
  if (partialResult == ABORTED)           \
    return error(CANCELED);               \
  else if (partialResult == ERROR)        \
    return error(FAILURE_DURING_HANDSHAKE);

static volatile char LINK_CABLE_MULTIBOOT_VERSION[] =
    "LinkCableMultiboot/v6.2.1";

const u8 LINK_CABLE_MULTIBOOT_CLIENT_IDS[] = {0b0010, 0b0100, 0b1000};

class LinkCableMultiboot {
 public:
  enum Result {
    SUCCESS,
    INVALID_SIZE,
    CANCELED,
    FAILURE_DURING_HANDSHAKE,
    FAILURE_DURING_TRANSFER
  };

  template <typename F>
  Result sendRom(const u8* rom, u32 romSize, F cancel) {
    if (romSize < LINK_CABLE_MULTIBOOT_MIN_ROM_SIZE)
      return INVALID_SIZE;
    if (romSize > LINK_CABLE_MULTIBOOT_MAX_ROM_SIZE)
      return INVALID_SIZE;
    if ((romSize % 0x10) != 0)
      return INVALID_SIZE;

    PartialResult partialResult;
    MultiBootParam multiBootParameters;
    multiBootParameters.client_data[0] = LINK_CABLE_MULTIBOOT_CLIENT_NO_DATA;
    multiBootParameters.client_data[1] = LINK_CABLE_MULTIBOOT_CLIENT_NO_DATA;
    multiBootParameters.client_data[2] = LINK_CABLE_MULTIBOOT_CLIENT_NO_DATA;
    multiBootParameters.palette_data = LINK_CABLE_MULTIBOOT_PALETTE_DATA;
    multiBootParameters.client_bit = 0;
    multiBootParameters.boot_srcp = (u8*)rom + LINK_CABLE_MULTIBOOT_HEADER_SIZE;
    multiBootParameters.boot_endp = (u8*)rom + romSize;

    LINK_CABLE_MULTIBOOT_TRY(detectClients(multiBootParameters, cancel))
    LINK_CABLE_MULTIBOOT_TRY(confirmClients(multiBootParameters, cancel))
    LINK_CABLE_MULTIBOOT_TRY(sendHeader(rom, cancel))
    LINK_CABLE_MULTIBOOT_TRY(confirmHeader(multiBootParameters, cancel))
    LINK_CABLE_MULTIBOOT_TRY(reconfirm(multiBootParameters, cancel))
    LINK_CABLE_MULTIBOOT_TRY(sendPalette(multiBootParameters, cancel))

    multiBootParameters.handshake_data = (LINK_CABLE_MULTIBOOT_HANDSHAKE_DATA +
                                          multiBootParameters.client_data[0] +
                                          multiBootParameters.client_data[1] +
                                          multiBootParameters.client_data[2]) %
                                         256;

    LINK_CABLE_MULTIBOOT_TRY(confirmHandshakeData(multiBootParameters, cancel))

    int result = MultiBoot(&multiBootParameters,
                           LINK_CABLE_MULTIBOOT_SWI_MULTIPLAYER_MODE);

    linkRawCable->deactivate();

    return result == 1 ? FAILURE_DURING_TRANSFER : SUCCESS;
  }

  ~LinkCableMultiboot() { delete linkRawCable; }

 private:
  LinkRawCable* linkRawCable = new LinkRawCable();

  enum PartialResult { NEEDS_RETRY, FINISHED, ABORTED, ERROR };

  struct Responses {
    u16 d[LINK_CABLE_MULTIBOOT_CLIENTS];
  };

  template <typename F>
  PartialResult detectClients(MultiBootParam& multiBootParameters, F cancel) {
    linkRawCable->activate(LINK_CABLE_MULTIBOOT_MAX_BAUD_RATE);

    for (u32 t = 0; t < LINK_CABLE_MULTIBOOT_DETECTION_TRIES; t++) {
      auto response =
          linkRawCable->transfer(LINK_CABLE_MULTIBOOT_HANDSHAKE, cancel);
      if (cancel())
        return ABORTED;

      for (u32 i = 0; i < LINK_CABLE_MULTIBOOT_CLIENTS; i++) {
        if ((response.data[1 + i] & 0xfff0) ==
            LINK_CABLE_MULTIBOOT_HANDSHAKE_RESPONSE) {
          auto clientId = response.data[1 + i] & 0xf;

          switch (clientId) {
            case 0b0010:
            case 0b0100:
            case 0b1000: {
              multiBootParameters.client_bit |= clientId;
              break;
            }
            default:
              return NEEDS_RETRY;
          }
        }
      }
    }

    if (multiBootParameters.client_bit == 0) {
      linkRawCable->deactivate();
      wait(LINK_CABLE_MULTIBOOT_WAIT_BEFORE_RETRY);
      return NEEDS_RETRY;
    }

    return FINISHED;
  }

  template <typename F>
  PartialResult confirmClients(MultiBootParam& multiBootParameters, F cancel) {
    return compare(
        multiBootParameters,
        LINK_CABLE_MULTIBOOT_CONFIRM_CLIENTS | multiBootParameters.client_bit,
        LINK_CABLE_MULTIBOOT_HANDSHAKE_RESPONSE, cancel);
  }

  template <typename F>
  PartialResult confirmHeader(MultiBootParam& multiBootParameters, F cancel) {
    return compare(multiBootParameters, LINK_CABLE_MULTIBOOT_HANDSHAKE, 0,
                   cancel);
  }

  template <typename F>
  PartialResult reconfirm(MultiBootParam& multiBootParameters, F cancel) {
    return compare(multiBootParameters, LINK_CABLE_MULTIBOOT_HANDSHAKE,
                   LINK_CABLE_MULTIBOOT_HANDSHAKE_RESPONSE, cancel);
  }

  template <typename F>
  PartialResult sendHeader(const u8* rom, F cancel) {
    u16* headerOut = (u16*)rom;

    for (int i = 0; i < LINK_CABLE_MULTIBOOT_HEADER_SIZE; i += 2) {
      linkRawCable->transfer(*(headerOut++), cancel);
      if (cancel())
        return ABORTED;
    }

    return FINISHED;
  }

  template <typename F>
  PartialResult sendPalette(MultiBootParam& multiBootParameters, F cancel) {
    auto data =
        LINK_CABLE_MULTIBOOT_SEND_PALETTE | LINK_CABLE_MULTIBOOT_PALETTE_DATA;

    auto response = linkRawCable->transfer(data, cancel);
    if (cancel())
      return ABORTED;

    for (u32 i = 0; i < LINK_CABLE_MULTIBOOT_CLIENTS; i++) {
      if (response.data[1 + i] >> 8 == LINK_CABLE_MULTIBOOT_ACK_RESPONSE)
        multiBootParameters.client_data[i] = response.data[1 + i] & 0xff;
    }

    for (u32 i = 0; i < LINK_CABLE_MULTIBOOT_CLIENTS; i++) {
      u8 clientId = LINK_CABLE_MULTIBOOT_CLIENT_IDS[i];
      bool isClientConnected = multiBootParameters.client_bit & clientId;

      if (isClientConnected && multiBootParameters.client_data[i] ==
                                   LINK_CABLE_MULTIBOOT_CLIENT_NO_DATA)
        return NEEDS_RETRY;
    }

    return FINISHED;
  }

  template <typename F>
  PartialResult confirmHandshakeData(MultiBootParam& multiBootParameters,
                                     F cancel) {
    u16 data = LINK_CABLE_MULTIBOOT_CONFIRM_HANDSHAKE_DATA |
               multiBootParameters.handshake_data;
    auto response = linkRawCable->transfer(data, cancel);
    if (cancel())
      return ABORTED;

    return (response.data[1] >> 8) == LINK_CABLE_MULTIBOOT_ACK_RESPONSE
               ? FINISHED
               : ERROR;
  }

  template <typename F>
  PartialResult compare(MultiBootParam& multiBootParameters,
                        u16 data,
                        u16 expectedResponse,
                        F cancel) {
    auto response = linkRawCable->transfer(data, cancel);
    if (cancel())
      return ABORTED;

    for (u32 i = 0; i < LINK_CABLE_MULTIBOOT_CLIENTS; i++) {
      u8 clientId = LINK_CABLE_MULTIBOOT_CLIENT_IDS[i];
      u16 expectedResponseWithId = expectedResponse | clientId;
      bool isClientConnected = multiBootParameters.client_bit & clientId;

      if (isClientConnected && response.data[1 + i] != expectedResponseWithId)
        return ERROR;
    }

    return FINISHED;
  }

  Result error(Result error) {
    linkRawCable->deactivate();
    return error;
  }

  void wait(u32 verticalLines) {
    u32 count = 0;
    u32 vCount = REG_VCOUNT;

    while (count < verticalLines) {
      if (REG_VCOUNT != vCount) {
        count++;
        vCount = REG_VCOUNT;
      }
    };
  }
};

extern LinkCableMultiboot* linkCableMultiboot;

#endif  // LINK_CABLE_MULTIBOOT_H
