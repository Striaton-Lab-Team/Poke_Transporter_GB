#include <tonc.h>
#include <stdarg.h>
#include <inttypes.h>
#include "libraries/nanoprintf/nanoprintf.h"
#include "libstd_replacements.h"
#include "link_handler.h"
#include "script_array.h"
#include "dbg/debug_mode.h"
#include "dbg/ptgb_mgba_print.h"
#include "interrupt.h"
#include "text_engine.h"
#include "global_frame_controller.h"
#include "background_engine.h"
#include "sprite_data.h"
#include "libraries/Pokemon-Gen3-to-Gen-X/include/save.h"
#include "flash_mem.h"
#include "FileContainerReader.h"
#include "text_tables.h"
#include "translated_text.h"
#include "gb_rom_values/base_gb_rom_struct.h"
#include "gb_rom_values/gb_rom_values.h"
#include "gb_rom_values_jpn_lz10_bin.h"
#include "gb_rom_values_eng_lz10_bin.h"
#include "gb_rom_values_fre_lz10_bin.h"
#include "gb_rom_values_ger_lz10_bin.h"
#include "gb_rom_values_ita_lz10_bin.h"
#include "gb_rom_values_spa_lz10_bin.h"
#include "gb_rom_values_kor_lz10_bin.h"

#include "GB_Payloads_chunk0_lz10_bin.h"
#include "GB_Payloads_chunk1_lz10_bin.h"
#include "GB_Payloads_chunk2_lz10_bin.h"
#include "GB_Payloads_chunk3_lz10_bin.h"
#include "GB_Payloads_chunk4_lz10_bin.h"
#include "GB_Payloads_chunk5_lz10_bin.h"
#include "GB_Payloads_chunk6_lz10_bin.h"

#include "GB_Payloads.h"

LinkSPI linkSPIInstance;
LinkSPI *linkSPI = &linkSPIInstance;

#define INITIAL_LINK_TIMER (0x4000 / 60)
#define DISCOVERY_TIMER_MIN 4
#define DISCOVERY_TIMER_PHASE_MASK 0x0F
#define DISCOVERY_TIMEOUT_TRANSFERS 2048
#define DISCOVERY_CONFIRM_TIMEOUT_TRANSFERS 180

// Here's a compilation check to ensure that the size of these structs match our expectations.
// Just update it if you changed the struct members. The data-generator process prints their actual sizes.
static_assert(sizeof(struct GB_ROM) == 140);
static_assert(sizeof(struct ROM_DATA) == 160);

LinkConnection globalLinkCable;

/**
 * @brief This function determines the expected packet arrival address.
 *
 * PTGB sends packets with an additional read pointer. For packets intended to run commands,
 * this address is set to the address the packet is supposed to arrive at.
 *
 * This address depends on the generation of the Game Boy ROM being used.
 */
static u16 determinePacketArrivalAddress(int gen)
{
  return (gen == 1) ? 0xC6DC : 0xC8DC;
}

void linkCableIRQ()
{
  /*
  ----------------
  This handshake process can be a bit weird, so let's break it down:

  First we exchange the bytes via handshake. inData will be set to the recieved byte, and the byte we send out will be outData.

  Then we determining what byte we will be sending out next, based on enterState and inByte in handleStateLogic().
  This will set exitState and nextOutByte.

  Then we print our information in the following format: globalCounter enterState:stateCounter:exitState inData outData

  We then prepare for the next cycle. Counters are incremented (or reset), enterState is set to exitState, and outByte is set to nextOutByte.
  ----------------
  */
  if (!globalLinkCable.earlyExit())
  {
    globalLinkCable.exchangeBytes();

    globalLinkCable.handleStateLogic();

    if (g_debug_options.print_link_data || g_debug_options.print_link_packets)
    {
      globalLinkCable.printData();
    }

    if (g_debug_options.write_cable_data_to_save)
    {
      globalLinkCable.writeData();
    }

    globalLinkCable.prepareForNextCycle();
  }
}

LinkPacket::LinkPacket(PayloadCommand cmd, byte arg1, byte arg2, u16 addr)
{
  command = cmd;
  argument[0] = arg1;
  argument[1] = arg2;
  pointer = addr;
  inUse = true;
};

void LinkPacket::loadSecondaryPayload(byte *payload, int payloadSize)
{
  secondaryPayloadData = payload;
  secondaryPayloadDataSize = payloadSize;
  for (int i = 0; i < payloadSize; i++)
  {
    secondaryPayloadSizeChecksum += payload[i];
  }
  secondaryPayloadSizeChecksum = 0x100 - secondaryPayloadSizeChecksum;

  PTGB_MGBA_INFO("Secondary payload loaded: dataSize=%d packetSizeArg=%d checksum=%X",
                 secondaryPayloadDataSize,
                 secondaryPayloadDataSize + SECONDARY_PAYLOAD_HEADER_SIZE,
                 secondaryPayloadSizeChecksum);
}

void LinkConnection::setup(const u16 *debug_charset)
{
  link_cable_memory_section_index = 0;
  link_cable_array_index = 0;
  writeBufferOffset = 0;
  pccsROMptr = nullptr;
  dataOutBufferCurrIndex = 0;

  linkSPI->activate(LinkSPI::Mode::MASTER_256KBPS);
  linkSPI->setWaitModeActive(false);

  this->debug_charset = debug_charset;

  lastError = NO_ERROR;

  if (g_debug_options.print_link_data == true)
  {
    create_textbox(0, 0, 138, 128, false);
  }

  if (g_debug_options.write_cable_data_to_save == WRITE_CABLE_DATA_MODE_SRAM)
  {
    // if we're writing the cable data to SRAM, we should clear the SRAM first to make sure there's no leftover data from previous transfers
    volatile u8 *cur = SRAM_PTR;
    volatile u8 *end = SRAM_PTR + 0x10000;
    while (cur < end)
    {
      (*cur) = 0;
      ++cur;
    }
  }
  else if (g_debug_options.write_cable_data_to_save == WRITE_CABLE_DATA_MODE_CART)
  {
    // before each write, we need to erase the sector.
    // so, let's do that for the first one before we start writing anything.
    erase_sector(0);
  }
}

void LinkConnection::loadPayload(GB_PayloadsFiles payload)
{
  u32 fileSize;
  u8 decompressionBuffer[0x1000];
  const u8 *chunkList[] = {
      (const u8 *)GB_Payloads_chunk0_lz10_bin,
      (const u8 *)GB_Payloads_chunk1_lz10_bin,
      (const u8 *)GB_Payloads_chunk2_lz10_bin,
      (const u8 *)GB_Payloads_chunk3_lz10_bin,
      (const u8 *)GB_Payloads_chunk4_lz10_bin,
      (const u8 *)GB_Payloads_chunk5_lz10_bin,
      (const u8 *)GB_Payloads_chunk6_lz10_bin,
  };
  FileContainerReader reader(chunkList, 7);
  const u32 fileIndex = (u32)payload;

  reader.init(decompressionBuffer, sizeof(decompressionBuffer));
  fileSize = reader.getFileSize(fileIndex);
  reader.seekToFile(fileIndex);
  reader.read(this->payloadBuffer, fileSize);

  this->curr_payload_size = fileSize;
}

void LinkConnection::loadPayloadByROM(GameBoyROM rom)
{
  loadPayload(GameBoyROMPayloads[rom]);
  lang = GameBoyROMLanguages[rom];
  vers = GameBoyROMVersions[rom];

  static GB_ROM gb_rom_values_buffer[7];
  const u8 *compressed_rom_values;

  switch (lang){
    case JAPANESE:
      compressed_rom_values = gb_rom_values_jpn_lz10_bin;
      break;
    default:
    case ENGLISH:
      compressed_rom_values = gb_rom_values_eng_lz10_bin;
      break;
    case FRENCH:
      compressed_rom_values = gb_rom_values_fre_lz10_bin;
      break;
    case ITALIAN:
      compressed_rom_values = gb_rom_values_ita_lz10_bin;
      break;
    case GERMAN:
      compressed_rom_values = gb_rom_values_ger_lz10_bin;
      break;
    case SPANISH:
      compressed_rom_values = gb_rom_values_spa_lz10_bin;
      break;
    case KOREAN:
      compressed_rom_values = gb_rom_values_kor_lz10_bin;
      break;
  }

  LZ77UnCompWram(compressed_rom_values, gb_rom_values_buffer);
  pccsROMptr = &gb_rom_values_buffer[vers];
}

void LinkConnection::loadCurrGameFromChecksum()
{
  if (((dataOutBuffer[0] + dataOutBuffer[1]) & 0x7F) != dataOutBuffer[2])
  {
    currROM = GB_ROM_ERROR;
    return;
  };

  int start = RED_JP_v0;
  int end = GOLD_JP_v0;

  if (gen == 2)
  {
    start = end;
    end = NO_GB_ROM;
  }

  for (int i = start; i < end; i++)
  {
    if (dataOutBuffer[0] == GameBoyROMChecksumTable[i][1] && dataOutBuffer[1] == GameBoyROMChecksumTable[i][2])
    {
      currROM = (GameBoyROM)GameBoyROMChecksumTable[i][3];
      return;
    }
  }
  currROM = GB_ROM_ERROR;
  return;
}

void LinkConnection::exchangeBytes()
{
  /*
  int timeout_frames = 10;
  inData = linkSPI->transfer(outData, [&timeout_frames]()
                             {
      // In the mGBA Lua bridge, replies arrive via emulator callbacks between frames.
      // Waiting here prevents valid bytes from being reported as timeouts.
      global_next_frame();
      return --timeout_frames <= 0; });
  */
  switch (g_debug_options.load_cable_data_from_save)
  {
  case WRITE_CABLE_DATA_MODE_OFF:
    // Normal transfer :-)
    inData = linkSPI->transfer(outData);
    // PTGB_MGBA_INFO("in: %X, out: %X", inData, outData);
    break;
  case WRITE_CABLE_DATA_MODE_SRAM:
    // Pretend transfer, by loading the bytes from SRAM (where we stored them with writeData() in a previous transfer)
    inData = (*(SRAM_PTR + link_cable_array_index));
    ++link_cable_array_index;
    outData = (*(SRAM_PTR + link_cable_array_index));
    ++link_cable_array_index;
    break;
  case WRITE_CABLE_DATA_MODE_CART:
  {
    // Pretend transfer, by loading the bytes from the cartridge save. (where we stored them with writeData() in a previous transfer)
    // skip the first 6 bytes, which are for human consumption
    link_cable_array_index += 6;

    inData = read_byte_save((0x1000 * link_cable_memory_section_index) + link_cable_array_index);
    ++link_cable_array_index;
    outData = read_byte_save((0x1000 * link_cable_memory_section_index) + link_cable_array_index);
    ++link_cable_array_index;

    if (link_cable_array_index >= 0x1000)
    {
      // if we reached the end of the section, we need to load the next section (if there is one)
      ++link_cable_memory_section_index;
      link_cable_array_index = 0;
    }
    break;
  }
  }
}

void LinkConnection::startConnection(LinkState startState)
{
  switch (startState)
  {
  case INITIAL_CONNECTION:
    // Start searching and make sure the first byte sent requests the external clock
    irq_disable(II_TIMER3);
    REG_TM3CNT = 0;
    REG_IF = (1 << II_TIMER3);
    linkSPI->activate(LinkSPI::Mode::MASTER_256KBPS);

    inData = 0xFF;
    outData = 0x01;
    nextOutData = 0x01;
    globalStateCounter = 0;
    subStateCounter = 0;
    subStateChanged = false;
    enterState = CLOCK_ACQUIRE;
    exitState = CLOCK_ACQUIRE;

    REG_TM3D = -DISCOVERY_TIMER_MIN;
    REG_TM3CNT = TM_FREQ_1024 | TM_IRQ | TM_ENABLE;
    break;
  case PACKET_EXCHANGE:
    REG_TM3D = -0x0040;
    // REG_TM3D = -0x4000 / 60;
    REG_TM3CNT = TM_FREQ_1024 | TM_IRQ | TM_ENABLE;
    break;
  default:
    break;
  }

  if (startState != INITIAL_CONNECTION)
  {
    enterState = startState;
  }
  irq_enable(II_TIMER3);
}

void LinkConnection::printData()
{
  if (globalLinkCable.skipPrint)
  {
    tte_erase_rect(0, 0, H_MAX, V_MAX);
  }
  else
  {
    if (g_debug_options.print_link_data)
    {
      n2hexstr(&line[0], globalStateCounter & 0xFFFF, 4);
      line[4] = '|';
      n2hexstr(&line[5], enterState & 0xFF, 2);
      line[7] = ':';
      n2hexstr(&line[8], subStateCounter & 0xFFFF, 4);
      line[12] = ':';
      n2hexstr(&line[13], exitState & 0xFF, 2);
      line[15] = '|';
      line[16] = 'i';
      n2hexstr(&line[17], inData & 0xFF, 2);
      line[19] = '|';
      line[20] = 'o';
      n2hexstr(&line[21], outData & 0xFF, 2);
      line[23] = '\0';
      scroll_text(true, tte_get_context(), false, 8, 8, 138, 135);
      ptgb_write_debug(this->debug_charset, line, true);
    }

    // TODO: This is pretty rough, but it's the best we can do until the text engine rewrite.
    if (g_debug_options.print_link_packets && currIncomingPacket != nullptr)
    {
      tte_erase_rect(160, 16, H_MAX, V_MAX);
      for (int i = 0; i < 4; i++)
      {
        for (int j = 0; j < 4; j++)
        {
          n2hexstr(&line[3 * j], dataOutBuffer[(4 * i) + j], 2);
          line[(3 * j) + 2] = ' ';
        }
        line[12] = '\0';
        tte_set_pos(160, 16 * i);
        ptgb_write_debug(this->debug_charset, line, true);
      }

      for (int i = 0; i < 4; i++)
      {
        byte tempBuffer[16];

        tempBuffer[0] = currIncomingPacket->packetID;
        tempBuffer[1] = currIncomingPacket->command;
        tempBuffer[2] = currIncomingPacket->pointer >> 0;
        tempBuffer[3] = currIncomingPacket->pointer >> 8;
        memcpy(&tempBuffer[4], currIncomingPacket->argument, 2);
        tempBuffer[6] = currIncomingPacket->latestError;
        tempBuffer[7] = 0x00;
        memcpy(&tempBuffer[8], currIncomingPacket->recievedData, 8);

        for (int j = 0; j < 4; j++)
        {
          n2hexstr(&line[3 * j], tempBuffer[(4 * i) + j], 2);
          line[(3 * j) + 2] = ' ';
        }
        line[12] = '\0';
        tte_set_pos(160, (16 * 5) + (16 * i));
        ptgb_write_debug(this->debug_charset, line, true);
      }
    }
  }
}

void LinkConnection::writeData()
{
  switch (g_debug_options.write_cable_data_to_save)
  {
  case WRITE_CABLE_DATA_MODE_OFF:
    break;
  case WRITE_CABLE_DATA_MODE_SRAM:
  {
    (*(SRAM_PTR + link_cable_array_index)) = inData;
    ++link_cable_array_index;

    (*(SRAM_PTR + link_cable_array_index)) = outData;
    ++link_cable_array_index;
    break;
  }
  case WRITE_CABLE_DATA_MODE_CART:
  {
    // save the data to the cartridge in chunks of 4 KB
    // WARNING: If you want to add or remove fields here,
    // make sure to keep the number of bytes a clean divider of 4096 (global_memory_buffer_size)

    // the next 6 bytes are for human consumption when viewed in a hex editor.
    // they can be useful to correlate the current LinkConnection state with the data that was sent over the cable.
    // but they're not needed for reconstructing the conversation with load_cable_data_from_save
    global_memory_buffer[writeBufferOffset + 0] = (u8)(globalStateCounter >> 8) & 0xFF;
    global_memory_buffer[writeBufferOffset + 1] = (u8)(globalStateCounter >> 0) & 0xFF;
    global_memory_buffer[writeBufferOffset + 2] = (u8)enterState;
    global_memory_buffer[writeBufferOffset + 3] = (u8)(subStateCounter >> 8) & 0xFF;
    global_memory_buffer[writeBufferOffset + 4] = (u8)(subStateCounter >> 0) & 0xFF;
    global_memory_buffer[writeBufferOffset + 5] = (u8)exitState;

    // actual data bytes start here.
    global_memory_buffer[writeBufferOffset + 6] = inData;
    global_memory_buffer[writeBufferOffset + 7] = outData;

    writeBufferOffset += 8;

    break;
  }
  }
}

void LinkConnection::handleStateLogic()
{
  switch (enterState)
  {
  case INITIAL_CONNECTION:
    nextOutData = 0x01;
    exitState = DISCOVERY_RESET;
    break;

  case DISCOVERY_RESET:
    // Reset and try a different timer phase
    REG_TM3CNT = 0;
    REG_IF = (1 << II_TIMER3);
    linkSPI->activate(LinkSPI::Mode::MASTER_256KBPS);
    outData = 0x01;
    nextOutData = 0x01;
    REG_TM3D = -(DISCOVERY_TIMER_MIN +
                 (globalStateCounter & DISCOVERY_TIMER_PHASE_MASK));
    REG_TM3CNT = TM_FREQ_1024 | TM_IRQ | TM_ENABLE;
    exitState = CLOCK_ACQUIRE;
    break;

  case CLOCK_ACQUIRE:
    nextOutData = 0x01;

    if (inData == 0x02)
    {
      // Receiving 0x02 while sending 0x01 means the other Game Boy offered
      // external-clock mode and should now have selected the GBA as master.
      REG_TM3D = -INITIAL_LINK_TIMER;
      exitState = CLOCK_CONFIRM;
    }
    else if (inData == 0xFE)
    {
      // Keep compatibility with an already-established connection where the
      // role-selection byte was completed before discovery began.
      REG_TM3D = -INITIAL_LINK_TIMER;
      exitState = SAVE_SUCCESS;
      nextOutData = 0x01;
    }
    else if (subStateCounter >= DISCOVERY_TIMEOUT_TRANSFERS)
    {
      exitState = DISCOVERY_RESET;
    }
    else
    {
      REG_TM3D = -(DISCOVERY_TIMER_MIN +
                   (subStateCounter & DISCOVERY_TIMER_PHASE_MASK));
    }
    break;

  case CLOCK_CONFIRM:
    if (inData == 0xFE)
    {
      exitState = SAVE_SUCCESS;
      nextOutData = 0x01;
    }
    else if (subStateCounter >= DISCOVERY_CONFIRM_TIMEOUT_TRANSFERS)
    {
      exitState = DISCOVERY_RESET;
      nextOutData = 0x01;
    }
    else
    {
      nextOutData = 0x01;
    }
    break;

  case SAVE_SUCCESS:
    nextOutData = 0x01;

    if (inData == 0x02)
    {
      // Don't exit if we already set the GB as the follower
    }
    else if (inData == 0x60 || inData == 0x61)
    {
      exitState = MENU_OPEN;
      nextOutData = inData;
    }
    break;

  case MENU_OPEN:
    if (inData == 0xD0 || inData == 0x61)
    {
      if (inData == 0xD0)
      {
        gen = 1;
        loadPayload(GB_PayloadsFiles::UNIVERSALPAYLOADGEN1);
        nextOutData = 0xD4;
      }
      else if (inData == 0x61)
      {
        gen = 2;
        loadPayload(GB_PayloadsFiles::UNIVERSALPAYLOADGEN2);
        nextOutData = 0x61;
      }
      exitState = MENU_SUCCESS;
    }
    break;

  case MENU_SUCCESS:
    if (inData == 0xFE)
    {
      exitState = WAIT_FOR_TRADE;
    }
    nextOutData = inData;
    break;

  case WAIT_FOR_TRADE:
    if (inData == 0xFD)
    {
      REG_TM3D = -0x0040;
      exitState = TRADE_PREAMBLE;
      // nextOutData defaults to 0x00
    }
    else
    {
      nextOutData = inData;
    }
    break;

  case TRADE_PREAMBLE:
    if (subStateCounter < 2)
    {
      // nextOutData defaults to 0x00
    }
    else if (subStateCounter < 9)
    {
      nextOutData = 0xFD;
    }
    else
    {
      exitState = TRADE;
      nextOutData = 0xFD;
    };
    break;

  case TRADE:
    if (subStateCounter >= curr_payload_size)
    {
      if (this->gen == 2)
      {
        exitState = MAIL;
      }
      else
      {
        exitState = WAIT_FOR_CHECKSUM_PAYLOAD;
      }
    }
      nextOutData = payloadBuffer[subStateCounter];
    break;

  case MAIL:
    if (subStateCounter >= 0x186)
    {
      exitState = WAIT_FOR_CHECKSUM_PAYLOAD;
    }
    nextOutData = 0x00;
    break;

  case WAIT_FOR_CHECKSUM_PAYLOAD:
    if (inData == 0xFD)
    {
      exitState = GET_CHECKSUM;
    }
    // nextOutData defaults to 0x00
    break;

  case GET_CHECKSUM:
    if (inData != 0xFD)
    {
      dataOutBuffer[dataOutBufferCurrIndex] = inData;
      dataOutBufferCurrIndex++;
      nextOutData = 0x01;
    }
    else if (inData == 0xFD && dataOutBufferCurrIndex > 0)
    {
      loadCurrGameFromChecksum();
      if (currROM == GB_ROM_ERROR)
      {
        while(true){}
        // Add in an error message thrown here once we, ya know, rewrite the text engine to make that possible
      }
      loadPayloadByROM(currROM);
      exitState = SEND_SPECIFIC_PAYLOAD;
    }
    else
    {
      nextOutData = 0xFD;
    }
    break;

  case WAIT_FOR_SECOND_PAYLOAD:
    if (inData == 0xFD)
    {
      exitState = SEND_SPECIFIC_PAYLOAD;
    }
    // nextOutData defaults to 0x00
    break;

  case SEND_SPECIFIC_PAYLOAD:
    if (subStateCounter >= 255) // The 255 comes from the Universal Payload
    {
      exitState = SEND_FIRST_PACKET;
    }
    if (subStateCounter < curr_payload_size)
    {
      nextOutData = payloadBuffer[subStateCounter];
    }
    else
    {
      nextOutData = 0x01;
    }
    break;

  case SEND_FIRST_PACKET:
    // In order to make sure everything is set to go, we need to send a dummy packet.
    currOutgoingPacket = &dummyPacket;
    nextOutData = 0xFF;
    exitState = STANDARD_PACKET_EXCHANGE;
    break;

  case PACKET_EXCHANGE:
    nextOutData = 0xFF;
    exitState = STANDARD_PACKET_EXCHANGE;
    break;

  case STANDARD_PACKET_EXCHANGE:
  {
    switch (subStateCounter)
    {
    case 0:
      nextOutData = 0xFD;
      break;
    case 1:
      nextOutData = linkPacketArrIndex;
      currOutgoingPacket->packetID = linkPacketArrIndex;
      break;
    case 2:
      nextOutData = currOutgoingPacket->command;
      if (currOutgoingPacket->command == CMD_SoftReset)
      {
        // Immedaitely remove the Soft Reset packet from in use, it will not get a response.
        currOutgoingPacket->inUse = false;
      }
      break;
    case 3:
      nextOutData = currOutgoingPacket->argument[0];
      break;
    case 4:
      nextOutData = currOutgoingPacket->argument[1];
      break;
    case 5:
      nextOutData = currOutgoingPacket->pointer >> 0;
      break;
    case 6:
      nextOutData = currOutgoingPacket->pointer >> 8;
      break;
    case TOTAL_PACKET_LENGTH:
      PTGB_MGBA_INFO("Sent packet: cmd=[%X], args=[%X, %X], ptr=[%X], ID=[%X], Data=[%X, %X, %X, %X, %X, %X, %X, %X]",
      currOutgoingPacket->command,
      currOutgoingPacket->argument[0],
      currOutgoingPacket->argument[1],
      currOutgoingPacket->pointer,
      currOutgoingPacket->packetID,
      currOutgoingPacket->recievedData[0],
      currOutgoingPacket->recievedData[1],
      currOutgoingPacket->recievedData[2],
      currOutgoingPacket->recievedData[3],
      currOutgoingPacket->recievedData[4],
      currOutgoingPacket->recievedData[5],
      currOutgoingPacket->recievedData[6],
      currOutgoingPacket->recievedData[7]
      );
      exitState = RUNNING_CODE_DELAY;
      nextOutData = 0xFF;
      break;
    default:
      nextOutData = 0xFF;
      break;
    }
  }
    dataOutBuffer[subStateCounter] = inData;
    break;
  
  case RUNNING_CODE_DELAY:
  {
    // We are currently processing something, we need to wait for that to finish.
    if (inData == 0xFE)
    {
      nextOutData = 0xFF;
    }
    else
    {
      if (currOutgoingPacket->secondaryPayloadData != nullptr)
      {
        // We get our response after the GB gets the secondary payload, send that first.
        exitState = SECONDARY_PACKET_EXCHANGE;
      }
      else
      {
        exitState = PROCESS_PACKET;
      }
    }
  }
  break;

  case SECONDARY_PACKET_EXCHANGE:
  {
    switch (subStateCounter)
    {
    case 0:
      nextOutData = 0xFD;
      PTGB_MGBA_INFO("Secondary byte: sub=%d phase=preamble out=%X in=%X packet=%X cmd=%d",
                     subStateCounter,
                     nextOutData,
                     inData,
                     currOutgoingPacket->packetID,
                     currOutgoingPacket->command);
      break;
    case 1:
      nextOutData = linkPacketArrIndex;
      currOutgoingPacket->packetID = linkPacketArrIndex;
      PTGB_MGBA_INFO("Secondary byte: sub=%d phase=counter out=%X in=%X packet=%X cmd=%d",
                     subStateCounter,
                     nextOutData,
                     inData,
                     currOutgoingPacket->packetID,
                     currOutgoingPacket->command);
      break;
    case 2:
      // The command ID matches the saftey byte (0xFF)
      nextOutData = 0xFF;
      PTGB_MGBA_INFO("Secondary byte: sub=%d phase=safety out=%X in=%X packet=%X cmd=%d",
                     subStateCounter,
                     nextOutData,
                     inData,
                     currOutgoingPacket->packetID,
                     currOutgoingPacket->command);
      break;
    case 3:
      nextOutData = currOutgoingPacket->secondaryPayloadDataSize + 1; // We need to include the checksum value here as well
      PTGB_MGBA_INFO("Secondary byte: sub=%d phase=dataSize out=%X in=%X dataSize=%d packetSizeArg=%d",
                     subStateCounter,
                     nextOutData,
                     inData,
                     currOutgoingPacket->secondaryPayloadDataSize,
                     currOutgoingPacket->secondaryPayloadDataSize + SECONDARY_PAYLOAD_HEADER_SIZE);
      break;
    default:
      // We're through all the constant values, let's do the rest.
      if ((subStateCounter - SECONDARY_PAYLOAD_HEADER_SIZE) < currOutgoingPacket->secondaryPayloadDataSize)
      {
        nextOutData = currOutgoingPacket->secondaryPayloadData[subStateCounter - SECONDARY_PAYLOAD_HEADER_SIZE];
        PTGB_MGBA_INFO("Secondary byte: sub=%d phase=data index=%d out=%X in=%X packet=%X",
                       subStateCounter,
                       subStateCounter - SECONDARY_PAYLOAD_HEADER_SIZE,
                       nextOutData,
                       inData,
                       currOutgoingPacket->packetID);
      }
      else if ((subStateCounter - SECONDARY_PAYLOAD_HEADER_SIZE) == currOutgoingPacket->secondaryPayloadDataSize)
      {
        // Add the checksum and we're done with the packet.
        nextOutData = currOutgoingPacket->secondaryPayloadSizeChecksum;
        PTGB_MGBA_INFO("Secondary byte: sub=%d phase=checksum out=%X in=%X packet=%X",
                       subStateCounter,
                       nextOutData,
                       inData,
                       currOutgoingPacket->packetID);
      }
      else if (inData != currOutgoingPacket->packetID)
      {
        // We need to wait until the packet is ready to be recieved.
        nextOutData = 0xFF;
      }
      else
      {
        PTGB_MGBA_INFO("Secondary response counter seen: sub=%d packet=%X cmd=%d in=%X",
                       subStateCounter,
                       currOutgoingPacket->packetID,
                       currOutgoingPacket->command,
                       inData);
        // Great, the secondary payload has been processed and we are currently receiving the response.
        // Let's log the first response byte and head back to the normal packet talk
        exitState = STANDARD_PACKET_EXCHANGE;

        // Use a dummy packet to flush it out
        currOutgoingPacket = &dummyPacket;
        nextOutData = 0xFF;
      }
    }
    break;
  }
  break;

  case PROCESS_PACKET:
    currIncomingPacket = &linkPacketArr[dataOutBuffer[INP_COUNTER_INDEX] % LINK_PACKET_ARRAY_SIZE];
    if (allPacketsProcessed())
    {
      // We have no valid responses that we are looking for, go to the end.
      exitState = END;
    }
    else if (processPacket())
    {
      // Processing succeeded, let's load the next one.
      exitState = LOAD_NEXT_PACKET;
    }
    else
    {
      // The processing failed. Let's move on and try again later
      prepareNextPacket();
      exitState = PACKET_EXCHANGE;
    }
    nextOutData = 0xFF;
    break;

  case LOAD_NEXT_PACKET:
    consumePacket();

    if (allPacketsProcessed())
    {
      exitState = END;
    }
    else
    {
      prepareNextPacket();
      exitState = PACKET_EXCHANGE;
    }
    nextOutData = 0xFF;
    break;

  case END:
    irq_disable(II_TIMER3);
    break;

  default:
    nextOutData = inData;
    break;
  }
}

void LinkConnection::prepareForNextCycle()
{
  if (exitState != enterState)
  {
    subStateCounter = 0;
    subStateChanged = true;
  }
  else
  {
    subStateCounter++;
    subStateChanged = false;
  }

  if (key_held(KEY_L) && key_held(KEY_R))
  {
    exitState = END;
  }

  globalStateCounter++;
  enterState = exitState;
  outData = nextOutData;
}

/*
 * So, dealing with writing to the cartridge in the IRQ handler was a no-go.
 * It just caused too many issues with data corruption, probably because the write and erase_sector
 * operation was taking too long.
 *
 * So, I moved it to the main loop through this function.
 * However, we need to be aware that this function can get interrupted by the IRQ handler at any time.
 *
 * We also need to take care to not have global_memory_buffer overflow as the IRQ handler just keeps adding to it.
 */
void LinkConnection::handleCartIO()
{
  u8 writeBuffer[0x1000];
  unsigned curBufDepth = writeBufferOffset;

  // first copy the current data to a local buffer. Note: the IRQ handler could append new data to global_memory_buffer during this call!
  memcpy(writeBuffer, global_memory_buffer, curBufDepth);
  // by updating the writeBufferOffset already, we allow the IRQ handler to start writing at the new right position
  // immediately.
  writeBufferOffset -= curBufDepth;
  // now move any data received during the memcpy to before writeBufferOffset, so it will be included in the next batch.
  // Note: keep in mind, here too the IRQ handler may be appending new data to global_memory_buffer and increase writeBufferOffset.
  // but it's harmless.
  memmove(global_memory_buffer, global_memory_buffer + curBufDepth, writeBufferOffset);

  u8 *curWriteBuf = writeBuffer;
  const u8 *const endWriteBuf = writeBuffer + curBufDepth;

  while (curWriteBuf < endWriteBuf)
  {
    // make sure not to write beyond the current flash sector's boundaries. We'll need an erase_sector() call before
    // we write to the next sector.
    const unsigned bytesRemainingInSector = 0x1000 - link_cable_array_index;
    const unsigned bytesToWrite = (curBufDepth < bytesRemainingInSector) ? curBufDepth : bytesRemainingInSector;

    copy_ram_to_save(curWriteBuf, (0x1000 * link_cable_memory_section_index) + link_cable_array_index, bytesToWrite);
    curWriteBuf += bytesToWrite;
    curBufDepth -= bytesToWrite;
    link_cable_array_index += bytesToWrite;

    if (link_cable_array_index >= 0x1000)
    {
      // we have reached the end of our current sector. Let's erase the next one.
      link_cable_array_index = 0;
      ++link_cable_memory_section_index;
      erase_sector(0x1000 * link_cable_memory_section_index);
    }
  }
}

bool LinkConnection::earlyExit()
{
  if (g_debug_options.print_link_data && !skipPrint && key_held(KEY_LEFT))
  {
    pauseOnByte = true;
    pauseOnPacket = false;
  }
  else if (g_debug_options.print_link_packets && !skipPrint && key_held(KEY_RIGHT))
  {
    pauseOnPacket = true;
    pauseOnByte = false;
  }
  else if (g_debug_options.print_link_data && !skipPrint && key_held(KEY_SELECT))
  {
    pauseOnByte = false;
  }
  else if (g_debug_options.print_link_packets && !skipPrint && key_held(KEY_START))
  {
    pauseOnPacket = false;
  }
  else if ((g_debug_options.print_link_data || g_debug_options.print_link_packets) && key_held(KEY_UP))
  {
    skipPrint = true;
    pauseOnPacket = false;
    pauseOnByte = false;
  }
  else if ((g_debug_options.print_link_data || g_debug_options.print_link_packets) && key_held(KEY_DOWN))
  {
    skipPrint = false;
  }

  if (pauseOnByte && g_debug_options.print_link_data)
  {
    if (key_hit(KEY_B))
    {
      return false; // Even if paused, run once
    }
  }

  if (pauseOnPacket && (enterState == LOAD_NEXT_PACKET) && g_debug_options.print_link_packets)
  {
    if (key_hit(KEY_A))
    {
      return false; // Even if paused, run once5
    }
  }

  return pauseOnByte || (pauseOnPacket && (enterState == LOAD_NEXT_PACKET));
}

bool LinkConnection::processPacket()
{

  PTGB_MGBA_INFO("Recieved raw data: Data=[%X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X]",
  dataOutBuffer[0],
  dataOutBuffer[1],
  dataOutBuffer[2],
  dataOutBuffer[3],
  dataOutBuffer[4],
  dataOutBuffer[5],
  dataOutBuffer[6],
  dataOutBuffer[7],
  dataOutBuffer[8],
  dataOutBuffer[9],
  dataOutBuffer[10],
  dataOutBuffer[11],
  dataOutBuffer[12],
  dataOutBuffer[13],
  dataOutBuffer[14],
  dataOutBuffer[15]
  );  
  
  int checksum = 0;
  if (currIncomingPacket->packetID != dataOutBuffer[INP_COUNTER_INDEX])
  {
    // This packet is not the correct ID for the response, ignore it
    PTGB_MGBA_INFO("Packet counter mismatch: expected=%u got=%u cmd=%d",
                   currIncomingPacket->packetID,
                   dataOutBuffer[INP_COUNTER_INDEX],
                   currIncomingPacket->command);
    return false;
  }

  for (int i = INP_COUNTER_INDEX; i < INP_LENGTH; i++)
  {
    if (i != INP_CHECKSUM_INDEX)
    {
      checksum += dataOutBuffer[i];
    }
  }
  // Add the read pointer
  checksum += ((currIncomingPacket->pointer + 8) >> 0) & 0xFF;
  checksum += ((currIncomingPacket->pointer + 8) >> 8) & 0xFF;

  checksum &= 0x7F;

  byte lsbByte = dataOutBuffer[INP_LSB_INDEX] | dataOutBuffer[INP_LSB_INDEX + 1];
  for (int i = 0; i < 8; i++)
  {
    currIncomingPacket->recievedData[i] =
        (dataOutBuffer[INP_DATA_INDEX + i] << 1) | ((lsbByte >> (7 - i)) & 0b1);
  }

  PTGB_MGBA_INFO("Packet decoded: packet=%u cmd=%d lsb=%X/%X checksumExpected=%X checksumGot=%X recv=%X %X %X %X %X %X %X %X",
                 currIncomingPacket->packetID,
                 currIncomingPacket->command,
                 dataOutBuffer[INP_LSB_INDEX],
                 dataOutBuffer[INP_LSB_INDEX + 1],
                 checksum,
                 dataOutBuffer[INP_CHECKSUM_INDEX],
                 currIncomingPacket->recievedData[0],
                 currIncomingPacket->recievedData[1],
                 currIncomingPacket->recievedData[2],
                 currIncomingPacket->recievedData[3],
                 currIncomingPacket->recievedData[4],
                 currIncomingPacket->recievedData[5],
                 currIncomingPacket->recievedData[6],
                 currIncomingPacket->recievedData[7]);

  if (checksum != dataOutBuffer[INP_CHECKSUM_INDEX])
  {
    // The checksum has to match in order for it to be valid, if we've made it this far down the line.
    currIncomingPacket->latestError = CHECKSUM_MISMATCH;
    PTGB_MGBA_INFO("Packet checksum mismatch: packet=%u cmd=%d expected=%X got=%X",
                   currIncomingPacket->packetID,
                   currIncomingPacket->command,
                   checksum,
                   dataOutBuffer[INP_CHECKSUM_INDEX]);
    return false;
  }

  if ((currIncomingPacket->command != CMD_ReadDataRequest) && (dataOutBuffer[INP_DATA_INDEX] & 0x80))
  {
    currIncomingPacket->latestError = ERROR_FLAG;
    PTGB_MGBA_INFO("Packet reports GB command failure: packet=%u cmd=%d encodedFirstData=%X decodedFirstData=%X",
                   currIncomingPacket->packetID,
                   currIncomingPacket->command,
                   dataOutBuffer[INP_DATA_INDEX],
                   currIncomingPacket->recievedData[0]);
    return false;
  }

  if (currIncomingPacket->command != CMD_ReadDataRequest)
  {
    // These command types will echo the first 8 bytes of the packet back to verify they were recieved correctly. Make sure that's the case.
    // This will also fail if a secondary packet fails
    if (
        currIncomingPacket->recievedData[0] != currIncomingPacket->packetID ||
        currIncomingPacket->recievedData[1] != currIncomingPacket->command ||
        currIncomingPacket->recievedData[2] != currIncomingPacket->argument[0] ||
        currIncomingPacket->recievedData[3] != currIncomingPacket->argument[1] ||
        currIncomingPacket->recievedData[4] != ((currIncomingPacket->pointer >> 0) & 0xFF) ||
        currIncomingPacket->recievedData[5] != ((currIncomingPacket->pointer >> 8) & 0xFF) ||
        currIncomingPacket->recievedData[6] != 0xFF ||
        currIncomingPacket->recievedData[7] != 0xFF)
    {
      currIncomingPacket->latestError = ECHO_MISMATCH;
      PTGB_MGBA_INFO("Packet echo mismatch: packet=%u cmd=%d recv=%X %X %X %X %X %X %X %X",
                     currIncomingPacket->packetID,
                     currIncomingPacket->command,
                     currIncomingPacket->recievedData[0],
                     currIncomingPacket->recievedData[1],
                     currIncomingPacket->recievedData[2],
                     currIncomingPacket->recievedData[3],
                     currIncomingPacket->recievedData[4],
                     currIncomingPacket->recievedData[5],
                     currIncomingPacket->recievedData[6],
                     currIncomingPacket->recievedData[7]);
      return false;
    }
  }

  currIncomingPacket->latestError = PACKET_SUCCESS;
  PTGB_MGBA_INFO("Packet success: packet=%u cmd=%d recv=%X %X %X %X %X %X %X %X",
                 currIncomingPacket->packetID,
                 currIncomingPacket->command,
                 currIncomingPacket->recievedData[0],
                 currIncomingPacket->recievedData[1],
                 currIncomingPacket->recievedData[2],
                 currIncomingPacket->recievedData[3],
                 currIncomingPacket->recievedData[4],
                 currIncomingPacket->recievedData[5],
                 currIncomingPacket->recievedData[6],
                 currIncomingPacket->recievedData[7]);
  return true;
}

void LinkConnection::consumePacket()
{
  /*
  ----------------
  For this part, the LinkConnection has an array of 4 packets that it will send.
  The current packet will be sent and a response will be recieved at the same time. If the recieved packet is
  successful, the data will be moved to the outArray and the packet will be replaced by a packet asking for the next data section.
  If the packet was not successful it will stay in the list and be ran again down the line. If packets have been sent for all the
  data sections, a completed packet will be replaced with an empty packet. Once the array is full of empty packets, the data
  has all been read and we are finished.
  ----------------
*/
  if (currIncomingPacket->latestError == PACKET_SUCCESS)
  {
    // Packet was successful, Find which packet sent it and mark it.
    // If this was data, then save the data and replace it with the next one in line
    currIncomingPacket->latestError = PACKET_READ;
    if (currIncomingPacket->command == CMD_ReadDataRequest)
    {
      for (int i = 0; i < 8; i++)
      {
        if (((currIncomingPacket->pointer - linkPacketDataStart) + i) < linkPacketDataSize)
        {
          outDataArrayPtr[(currIncomingPacket->pointer - linkPacketDataStart) + i] = currIncomingPacket->recievedData[i];
        }
      }
      // Do we have more data to read?
      if ((linkPacketDataAddr - linkPacketDataStart) < linkPacketDataSize)
      {
        *currIncomingPacket = LinkPacket(CMD_ReadDataRequest, 0x00, 0x00, linkPacketDataAddr);
        linkPacketDataAddr += 8;
      }
      else
      {
        // Mark the packet as out of use, we've gotten all the data we need.
        currIncomingPacket->inUse = false;
      }
    }
    else
    {
      // Command was not a data request, don't do anything.
    }
  }
  else
  {
    // Packet failed, don't remove it.
  }
}

bool LinkConnection::prepareNextPacket()
{
  for (int i = 0; i < LINK_PACKET_ARRAY_SIZE - 1; i++)
  {
    linkPacketArrIndex = (linkPacketArrIndex + 1) & LINK_PACKET_INDEX_MASK;

    LinkPacket *candidate = &linkPacketArr[linkPacketArrIndex % LINK_PACKET_ARRAY_SIZE];

    if (candidate->inUse && candidate->latestError != PACKET_READ)
    {
      currOutgoingPacket = candidate;
      return true;
    }
  }
  // We're back where we started. Send the dummy packet to get a response.
  currOutgoingPacket = &dummyPacket;
  return false;
}

bool LinkConnection::allPacketsProcessed()
{
  for (int i = 0; i < LINK_PACKET_ARRAY_SIZE; i++)
  {
    if (linkPacketArr[i].inUse && linkPacketArr[i].latestError != PACKET_READ)
    {
      return false;
    }
  }
  return true;
}

void LinkConnection::resetLinkPackets()
{
  for (int i = 0; i < LINK_PACKET_ARRAY_SIZE; i++)
  {
    linkPacketArr[i] = LinkPacket();
  }

  // Loop the packet index around back to 0
  while (linkPacketArrIndex % LINK_PACKET_ARRAY_SIZE != 0)
  {
    linkPacketArrIndex = (linkPacketArrIndex + 1) & LINK_PACKET_INDEX_MASK;
  }

  // Set the current outgoing packet to be the first in order.
  currOutgoingPacket = &linkPacketArr[0];
  // And set the incoming packet to nullptr since we don't know what that is yet.
  currIncomingPacket = nullptr;

  for (int i = 0; i < 16; i++)
  {
    dataOutBuffer[i] = 0;
  }
}

void LinkConnection::waitForEnd()
{
  while (enterState != END)
  {
    handleCartIO();
    VBlankIntrWait();
  }
}

bool LinkConnection::LinkCommand_InitalizeConnection(bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }

  PTGB_MGBA_INFO("Running command: InitalizeConnection");

  globalLinkCable.startConnection(INITIAL_CONNECTION);

  if (waitForCompletion)
  {
    waitForEnd();
  }

  return true;
}

bool LinkConnection::LinkCommand_ReloadCurrentBox(bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }
  
  PTGB_MGBA_INFO("Running command: ReloadCurrentBox");

  resetLinkPackets();

  linkPacketArr[0] = LinkPacket(CMD_ReloadCurrentBox, 0x00, 0x00, determinePacketArrivalAddress(gen));

  globalLinkCable.startConnection(PACKET_EXCHANGE);
  if (waitForCompletion)
  {
    waitForEnd();
  }
  return true;
};

bool LinkConnection::LinkCommand_TransferPokemon(int boxNumber, byte removalArray[], int removalArrayLength, bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }

  int maxBoxes = 0;
  if (gen == 1)
  {
    if (lang == JAPANESE)
    {
      maxBoxes = 8;
    }
    else
    {
      maxBoxes = 12;
    }
  }
  else
  {
        if (lang == JAPANESE)
    {
      maxBoxes = 9;
    }
    else
    {
      maxBoxes = 14;
    }
  }

  if (boxNumber >= maxBoxes)
  {
    return false;
  }
  
  PTGB_MGBA_INFO("Running command: TransferPokemon");

  resetLinkPackets();

  linkPacketArr[0] = LinkPacket(CMD_TransferPokemon, removalArrayLength + SECONDARY_PAYLOAD_HEADER_SIZE, boxNumber, determinePacketArrivalAddress(gen));
  linkPacketArr[0].loadSecondaryPayload(removalArray, removalArrayLength);

  // REG_TM3D = -0x4000 / 60;

  globalLinkCable.startConnection(PACKET_EXCHANGE);
  if (waitForCompletion)
  {
    waitForEnd();
  }
  return true;
};

bool LinkConnection::LinkCommand_SoftReset(bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }

  PTGB_MGBA_INFO("Running command: SoftReset");

  resetLinkPackets();

  linkPacketArr[0] = LinkPacket(CMD_SoftReset, 0x00, 0x00, determinePacketArrivalAddress(gen));

  globalLinkCable.startConnection(PACKET_EXCHANGE);
  if (waitForCompletion)
  {
    waitForEnd();
  }
  return true;
};

bool LinkConnection::LinkCommand_ModifySRAMAccess(bool enableSRAM, byte SRAMbank, bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }
  
  if (SRAMbank > 3)
  {
    return false;
  }

  PTGB_MGBA_INFO("Running command: ModifySRAMAccess");

  resetLinkPackets();

  if (enableSRAM)
  {
    linkPacketArr[0] = LinkPacket(CMD_ModifySRAMAccess, 0x0A, SRAMbank, determinePacketArrivalAddress(gen));
  }
  else
  {
    linkPacketArr[0] = LinkPacket(CMD_ModifySRAMAccess, 0x00, 0x00, determinePacketArrivalAddress(gen));
  }

  globalLinkCable.startConnection(PACKET_EXCHANGE);
  if (waitForCompletion)
  {
    waitForEnd();
  }
  return true;
};

bool LinkConnection::LinkCommand_RunSecondaryPayload(byte payload[], int payloadLength, bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }

  PTGB_MGBA_INFO("Running command: RunSecondaryPayload");

  resetLinkPackets();

  linkPacketArr[0] = LinkPacket(CMD_RunSecondaryPayload, payloadLength + SECONDARY_PAYLOAD_HEADER_SIZE, 0x00, determinePacketArrivalAddress(gen));
  linkPacketArr[0].loadSecondaryPayload(payload, payloadLength);

  globalLinkCable.startConnection(PACKET_EXCHANGE);
  if (waitForCompletion)
  {
    waitForEnd();
  }
  return true;
};

bool LinkConnection::LinkCommand_ReadMemorySection(u32 dataPointer, byte outArray[], int outArraySize, bool waitForCompletion)
{
  if (g_debug_options.ignore_link_cable)
  {
    return true;
  }
  else if (!linkSPI->isActive())
  {
    return false;
  }

  if (dataPointer > 0xFFFF)
  {
    waitForCompletion = true; // We have to wait for completion with the SRAM bank loading, so we can close it
    LinkCommand_ModifySRAMAccess(true, dataPointer >> 16);
  }

  PTGB_MGBA_INFO("Running command: ReadMemorySection");

  resetLinkPackets();

  linkPacketDataStart = dataPointer;
  linkPacketDataSize = outArraySize;
  linkPacketDataAddr = dataPointer & 0xFFFF;
  outDataArrayPtr = outArray;

  for (int i = 0; i < LINK_PACKET_ARRAY_SIZE; i++)
  {
    if ((i * 8) < linkPacketDataSize)
    {
      linkPacketArr[i] = LinkPacket(CMD_ReadDataRequest, 0x00, 0x00, linkPacketDataAddr);
      linkPacketDataAddr += 8;
    }
    else
    {
      linkPacketArr[i] = dummyPacket;
    }
  }

  globalLinkCable.startConnection(PACKET_EXCHANGE);
  if (waitForCompletion)
  {
    waitForEnd();
  }

  LinkCommand_ModifySRAMAccess(false, dataPointer >> 16);

  return true;
}