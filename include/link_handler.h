#ifndef LINK_HANDLER_H_
#define LINK_HANDLER_H_

#include <tonc.h>
#include "libraries/gba-link-connection/LinkSPI.hpp"
#include "GB_Payloads.h"
#include "typeDefs.h"
#include "gb_rom_values/base_gb_rom_struct.h"

#define SPI_TEXT_OUT_ARRAY_ELEMENT_SIZE 64

enum GameBoyROM
{
    RED_JP_v0,
    RED_JP_v1,
    RED_EN,
    RED_FR,
    RED_IT,
    RED_DE,
    RED_SP,
    GREEN_JP_v0,
    GREEN_JP_v1,
    BLUE_JP,
    BLUE_EN,
    BLUE_FR,
    BLUE_IT,
    BLUE_DE,
    BLUE_SP,
    YELLOW_JP_v0,
    YELLOW_JP_v1,
    YELLOW_JP_v2,
    YELLOW_JP_v3,
    YELLOW_EN,
    YELLOW_FR,
    YELLOW_IT,
    YELLOW_DE,
    YELLOW_SP,
    GOLD_JP_v0,
    GOLD_JP_v1,
    GOLD_EN,
    GOLD_FR,
    GOLD_IT,
    GOLD_DE,
    GOLD_SP,
    GOLD_KOR,
    SILVER_JP_v0,
    SILVER_JP_v1,
    SILVER_EN,
    SILVER_FR,
    SILVER_IT,
    SILVER_DE,
    SILVER_SP,
    SILVER_KOR,
    CRYSTAL_JP,
    CRYSTAL_EN_v0,
    CRYSTAL_EN_v1,
    CRYSTAL_EN_vA,
    CRYSTAL_FR,
    CRYSTAL_IT,
    CRYSTAL_DE,
    CRYSTAL_SP,

    NO_GB_ROM,
    GB_ROM_ERROR,
};

const GB_PayloadsFiles GameBoyROMPayloads[] =
    {
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_R,    // RED_JP_v0
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_R11,  // RED_JP_v1
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_EN_R,    // RED_EN
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_FR_R,    // RED_FR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_IT_R,    // RED_IT
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_DE_R,    // RED_DE
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_SP_R,    // RED_SP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_GR,   // GREEN_JP_v0
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_GR11, // GREEN_JP_v1
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_B,    // BLUE_JP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_EN_B,    // BLUE_EN
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_FR_B,    // BLUE_FR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_IT_B,    // BLUE_IT
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_DE_B,    // BLUE_DE
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_SP_B,    // BLUE_SP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_Y,    // YELLOW_JP_v0
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_Y11,  // YELLOW_JP_v1
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_Y12,  // YELLOW_JP_v2
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_JP_Y13,  // YELLOW_JP_v3
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_EN_Y,    // YELLOW_EN
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_FR_Y,    // YELLOW_FR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_IT_Y,    // YELLOW_IT
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_DE_Y,    // YELLOW_DE
        GB_PayloadsFiles::SPECIFICPAYLOADGEN1_SP_Y,    // YELLOW_SP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_JP_G,    // GOLD_JP_v0
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_JP_G11,  // GOLD_JP_v1
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_EN_G,    // GOLD_EN
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_FR_G,    // GOLD_FR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_IT_G,    // GOLD_IT
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_DE_G,    // GOLD_DE
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_SP_G,    // GOLD_SP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_KOR_G,   // GOLD_KOR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_JP_S,    // SILVER_JP_v0
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_JP_S11,  // SILVER_JP_v1
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_EN_S,    // SILVER_EN
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_FR_S,    // SILVER_FR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_IT_S,    // SILVER_IT
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_DE_S,    // SILVER_DE
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_SP_S,    // SILVER_SP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_KOR_S,   // SILVER_KOR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_JP_C,    // CRYSTAL_JP
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_EN_C,    // CRYSTAL_EN_v0
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_EN_C11,  // CRYSTAL_EN_v1
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_EN_CAU,  // CRYSTAL_EN_vA
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_FR_C,    // CRYSTAL_FR
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_IT_C,    // CRYSTAL_IT
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_DE_C,    // CRYSTAL_DE
        GB_PayloadsFiles::SPECIFICPAYLOADGEN2_SP_C,    // CRYSTAL_SP
};

const Language GameBoyROMLanguages[] =
    {
        JAPANESE, // RED_JP_v0
        JAPANESE, // RED_JP_v1
        ENGLISH,  // RED_EN
        FRENCH,   // RED_FR
        ITALIAN,  // RED_IT
        GERMAN,   // RED_DE
        SPANISH,  // RED_SP
        JAPANESE, // GREEN_JP_v0
        JAPANESE, // GREEN_JP_v1
        JAPANESE, // BLUE_JP
        ENGLISH,  // BLUE_EN
        FRENCH,   // BLUE_FR
        ITALIAN,  // BLUE_IT
        GERMAN,   // BLUE_DE
        SPANISH,  // BLUE_SP
        JAPANESE, // YELLOW_JP_v0
        JAPANESE, // YELLOW_JP_v1
        JAPANESE, // YELLOW_JP_v2
        JAPANESE, // YELLOW_JP_v3
        ENGLISH,  // YELLOW_EN
        FRENCH,   // YELLOW_FR
        ITALIAN,  // YELLOW_IT
        GERMAN,   // YELLOW_DE
        SPANISH,  // YELLOW_SP
        JAPANESE, // GOLD_JP_v0
        JAPANESE, // GOLD_JP_v1
        ENGLISH,  // GOLD_EN
        FRENCH,   // GOLD_FR
        ITALIAN,  // GOLD_IT
        GERMAN,   // GOLD_DE
        SPANISH,  // GOLD_SP
        KOREAN,   // GOLD_KOR
        JAPANESE, // SILVER_JP_v0
        JAPANESE, // SILVER_JP_v1
        ENGLISH,  // SILVER_EN
        FRENCH,   // SILVER_FR
        ITALIAN,  // SILVER_IT
        GERMAN,   // SILVER_DE
        SPANISH,  // SILVER_SP
        KOREAN,   // SILVER_KOR
        JAPANESE, // CRYSTAL_JP
        ENGLISH,  // CRYSTAL_EN_v0
        ENGLISH,  // CRYSTAL_EN_v1
        ENGLISH,  // CRYSTAL_EN_vA
        FRENCH,   // CRYSTAL_FR
        ITALIAN,  // CRYSTAL_IT
        GERMAN,   // CRYSTAL_DE
        SPANISH,  // CRYSTAL_SP
};

// This would probably make more sense as a "greater than less than" check.
const Version GameBoyROMVersions[] =
    {
            RED,     // RED_JP_v0
            RED,     // RED_JP_v1
            RED,     // RED_EN
            RED,     // RED_FR
            RED,     // RED_IT
            RED,     // RED_DE
            RED,     // RED_SP
            GREEN,   // GREEN_JP_v0
            GREEN,   // GREEN_JP_v1
            BLUE,    // BLUE_JP
            BLUE,    // BLUE_EN
            BLUE,    // BLUE_FR
            BLUE,    // BLUE_IT
            BLUE,    // BLUE_DE
            BLUE,    // BLUE_SP
            YELLOW,  // YELLOW_JP_v0
            YELLOW,  // YELLOW_JP_v1
            YELLOW,  // YELLOW_JP_v2
            YELLOW,  // YELLOW_JP_v3
            YELLOW,  // YELLOW_EN
            YELLOW,  // YELLOW_FR
            YELLOW,  // YELLOW_IT
            YELLOW,  // YELLOW_DE
            YELLOW,  // YELLOW_SP
            GOLD,    // GOLD_JP_v0
            GOLD,    // GOLD_JP_v1
            GOLD,    // GOLD_EN
            GOLD,    // GOLD_FR
            GOLD,    // GOLD_IT
            GOLD,    // GOLD_DE
            GOLD,    // GOLD_SP
            GOLD,    // GOLD_KOR
            SILVER,  // SILVER_JP_v0
            SILVER,  // SILVER_JP_v1
            SILVER,  // SILVER_EN
            SILVER,  // SILVER_FR
            SILVER,  // SILVER_IT
            SILVER,  // SILVER_DE
            SILVER,  // SILVER_SP
            SILVER,  // SILVER_KOR
            CRYSTAL, // CRYSTAL_JP
            CRYSTAL, // CRYSTAL_EN_v0
            CRYSTAL, // CRYSTAL_EN_v1
            CRYSTAL, // CRYSTAL_EN_vA
            CRYSTAL, // CRYSTAL_FR
            CRYSTAL, // CRYSTAL_IT
            CRYSTAL, // CRYSTAL_DE
            CRYSTAL, // CRYSTAL_SP
};

// This table has the 3 checksums, followed by the enum value
const u8 GameBoyROMChecksumTable[][4]{
    {0x32, 0xA2, 0xC1, RED_JP_v0},
    {0x31, 0xB8, 0x66, RED_JP_v1},
    {0x20, 0x91, 0xE6, RED_EN},
    {0x18, 0x7A, 0xFC, RED_FR},
    {0x18, 0x89, 0xD2, RED_IT},
    {0x18, 0x5C, 0xDC, RED_DE},
    {0x18, 0x38, 0x4A, RED_SP},
    {0x9C, 0xDD, 0xD5, GREEN_JP_v0},
    {0x9B, 0xF5, 0x47, GREEN_JP_v1},
    {0xE5, 0xDC, 0x36, BLUE_JP},
    {0xD3, 0x9D, 0x0A, BLUE_EN},
    {0xCB, 0x56, 0xA4, BLUE_FR},
    {0xCB, 0x5E, 0x9C, BLUE_IT},
    {0xCB, 0x2E, 0xBC, BLUE_DE},
    {0xCB, 0x14, 0xD7, BLUE_SP},
    {0x20, 0x9C, 0x29, YELLOW_JP_v0},
    {0x1F, 0x88, 0x58, YELLOW_JP_v1},
    {0x1E, 0xED, 0xD9, YELLOW_JP_v2},
    {0x1D, 0xD9, 0x84, YELLOW_JP_v3},
    {0x97, 0x04, 0x7C, YELLOW_EN},
    {0x61, 0x66, 0xFB, YELLOW_FR},
    {0x5C, 0x4E, 0x8F, YELLOW_IT},
    {0x5F, 0xB7, 0xC1, YELLOW_DE},
    {0x52, 0x56, 0x37, YELLOW_SP},
    {0x48, 0x8A, 0x70, GOLD_JP_v0},
    {0x47, 0x84, 0x60, GOLD_JP_v1},
    {0x4B, 0x68, 0x2D, GOLD_EN},
    {0x4A, 0x6F, 0xC6, GOLD_FR},
    {0x47, 0xCE, 0x0C, GOLD_IT},
    {0x4C, 0xDC, 0x97, GOLD_DE},
    {0x3D, 0x93, 0x53, GOLD_SP},
    {0x08, 0x77, 0x8A, GOLD_KOR},
    {0x27, 0x76, 0x91, SILVER_JP_v0},
    {0x26, 0x1D, 0x34, SILVER_JP_v1},
    {0x2A, 0x0D, 0xAE, SILVER_EN},
    {0x29, 0xFB, 0x8C, SILVER_FR},
    {0x26, 0x73, 0x50, SILVER_IT},
    {0x2B, 0xCD, 0x6E, SILVER_DE},
    {0x1C, 0x06, 0x4B, SILVER_SP},
    {0xE7, 0x98, 0x5B, SILVER_KOR},
    {0x22, 0x9A, 0x40, CRYSTAL_JP},
    {0x27, 0x12, 0x9F, CRYSTAL_EN_v0},
    {0x26, 0x18, 0xD2, CRYSTAL_EN_v1},
    {0x17, 0xAF, 0x7C, CRYSTAL_EN_vA},
    {0x26, 0xF2, 0xE2, CRYSTAL_FR},
    {0x23, 0xDB, 0xBA, CRYSTAL_IT},
    {0x28, 0x49, 0x82, CRYSTAL_DE},
    {0x19, 0x42, 0xF4, CRYSTAL_SP},
};

enum LinkState
{
    INITIAL_CONNECTION = 0x00,
    DISCOVERY_RESET,
    CLOCK_ACQUIRE,
    CLOCK_CONFIRM,
    SAVE_SUCCESS,
    MENU_OPEN,
    MENU_SUCCESS,
    WAIT_FOR_TRADE,
    TRADE_PREAMBLE,
    TRADE,
    MAIL,
    WAIT_FOR_CHECKSUM_PAYLOAD,
    GET_CHECKSUM,
    WAIT_FOR_SECOND_PAYLOAD,
    SEND_SPECIFIC_PAYLOAD,
    SEND_FIRST_PACKET,

    PACKET_EXCHANGE = 0x10,
    STANDARD_PACKET_EXCHANGE,
    RUNNING_CODE_DELAY,
    SECONDARY_PACKET_EXCHANGE,
    PROCESS_PACKET,
    LOAD_NEXT_PACKET,

    END = 0xFF,
};

enum LinkConnectionError
{
    NO_ERROR,
    PACKET_TIMED_OUT,
    CHECKSUM_MISMATCH,
    ECHO_MISMATCH,
    ERROR_FLAG,

    PACKET_SUCCESS,
    PACKET_READ,
};

enum PayloadCommand
{
    CMD_NONE = 0x7F,             // Using 0x7F instead of -1, since that ends up being 0xFE.
    CMD_ReloadCurrentBox = 0,    // no arguments used. Reloads the current box from SRAM. Use this as the first command byte before performing other commands.
    CMD_TransferPokemon,         // 1st argument = secondary payload size, 2nd argument = box that should be transferred from. This command uses a secondary payload. The size of this secondary payload needs to be declared beforehand. Prior to requesting a secondary payload, the program will use the second argument to load a specific box from SRAM. Box numbers are 0-indexed and range from 0x00 (box 1) to 0x0B(box 12). This load procedure currently cannot be skipped. Once the secondary payload arrives, the program will verify its integrity and align the payload. Afterwards, it will use the information within this payload to remove pokémon from the current box. Once all transferred pokémon have been removed, the program will save the current box.
    CMD_SoftReset,               // no arguments used. Instantly soft resets the game.
    CMD_ModifySRAMAccess,        // 1st argument = SRAM dis/enable. set to 0x0A to open access, set to 0x00 to close access. Second argument = SRAM bank. When closing SRAM access, set to 0x00. SRAM remains open until closed by this command or until closed by CMD_TransferPokemon/CMD_ReloadCurrentBox.
    CMD_RunSecondaryPayload,     // 1st argument = secondary payload size. This command uses a secondary payload. The size of this secondary payload needs to be declared beforehand. Once the secondary payload arrives, the program will verify its integrity and align the payload. Afterwards, it will jump to the secondary payload and execute it. IMPORTANT: if you want to report an error when executing this secondary payload, return with carry flag set. IMPORTANT: if you want to report that the payload was executed safely, return with carry flag reset.*/
    CMD_ReadDataRequest,         // a request to read data starting from the address defined by the 16-bit pointer.
    CMD_SecondaryPayload = 0xFF, // Used whenever a packet is for a secondary payload.
};

#define OUTP_PREAMBLE_INDEX 0
#define OUTP_COUNTER_INDEX 1
#define OUTP_COMMAND_INDEX 2
#define OUTP_ARGS_INDEX 3
#define OUTP_POINTER_INDEX 5
#define OUTP_FILLER_INDEX 7
#define OUTP_LENGTH 12

#define INP_COUNTER_INDEX (0 + INP_DELAY_FROM_OUTP)
#define INP_LSB_INDEX (1 + INP_DELAY_FROM_OUTP)
#define INP_CHECKSUM_INDEX (3 + INP_DELAY_FROM_OUTP)
#define INP_DATA_INDEX (4 + INP_DELAY_FROM_OUTP)
#define INP_LENGTH (12 + INP_DELAY_FROM_OUTP)

#define INP_DELAY_FROM_OUTP 3
#define TOTAL_PACKET_LENGTH (OUTP_LENGTH + INP_DELAY_FROM_OUTP)

struct LinkPacket
{
    bool inUse = false;

    // Out packet parameters
    PayloadCommand command = CMD_NONE;
    byte argument[2] = {0x00, 0x00};
    u16 pointer = 0xC6DC;
    byte packetID = 0x7F;

    // Incoming data
    LinkConnectionError latestError = NO_ERROR;
    byte recievedData[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    // Secondary payload
    byte *secondaryPayloadData = nullptr;
    int secondaryPayloadDataSize = 0;
    byte secondaryPayloadSizeChecksum = 0;
#define SECONDARY_PAYLOAD_HEADER_SIZE 4 // The size of a secondary packet, not including the data bytes or checksum

    LinkPacket() {};
    LinkPacket(PayloadCommand cmd, byte arg1, byte arg2, u16 addr);
    void loadSecondaryPayload(byte *payload, int paylHeoadSize);
};

class LinkConnection
{
public:
    LinkState enterState;
    LinkState exitState;
    bool subStateChanged = false;

    LinkConnectionError lastError = NO_ERROR;

    uint8_t inData = 0xFF;
    uint8_t outData = 0xFF;
    uint8_t nextOutData = 0xFF;

    int globalStateCounter = 0; // The counter for the total number of bytes sent
    int subStateCounter = 0;    // The counter for the total number of bytes sent in this substate

    int gen = 0;                      // The generation we are trading with
    Language lang = LANGUAGE_UNKNOWN; // The language we are trading with
    Version vers = VERSION_UNKNOWN;   // The version we are trading with
    GameBoyROM currROM = NO_GB_ROM;   // The GameBoy ROM we're communicating with
    const GB_ROM *pccsROMptr = nullptr;     // A pointer to the ROM data for the game we're communicating with
    byte boxNum = 0;                    // The current box the user has selected

    int FF_count = 0;   // The number of 0xFF bytes that have been in a row
    int zero_count = 0; // The number of 0x00 bytes that have been in a row

    byte payloadBuffer[0x2A0];
    int curr_payload_size = 0;
    byte dataOutBuffer[16];
    int dataOutBufferCurrIndex = 0;

    // This MUST be a power of 2!
#define LINK_PACKET_ARRAY_SIZE 4

// This is lower than the max 0x7F so we can save that value for dummy packets
#define LINK_PACKET_INDEX_MASK 0x3F

    LinkPacket linkPacketArr[LINK_PACKET_ARRAY_SIZE];
    LinkPacket *currOutgoingPacket;
    LinkPacket *currIncomingPacket;
    LinkPacket dummyPacket = LinkPacket();
    byte linkPacketArrIndex = LINK_PACKET_ARRAY_SIZE; // We want to avoid starting at zero, since that is a common value sent back in error.
    u16 linkPacketDataAddr = 0;
    u16 linkPacketDataStart = 0;
    int linkPacketDataSize = 0;
    byte *outDataArrayPtr;

    bool pauseOnByte = false;   // Used for pausing and sending one byte at a time
    bool pauseOnPacket = false; // Used for pausing and sending one packet at a time
    bool skipPrint = false;     // Skips printing to the screen

    void setup(const u16 *debug_charset);
    void startConnection(LinkState startState);
    bool earlyExit();
    void exchangeBytes();
    void printData();
    void writeData();
    void handleStateLogic();
    void prepareForNextCycle();
    void resetLinkPackets();

    // These are all the Link Commands
    bool LinkCommand_InitalizeConnection(bool waitForCompletion = true);
    bool LinkCommand_ReloadCurrentBox(bool waitForCompletion = true);
    bool LinkCommand_TransferPokemon(int boxNumber, byte removalArray[], int removalArrayLength, bool waitForCompletion = true);
    bool LinkCommand_SoftReset(bool waitForCompletion = true);
    bool LinkCommand_ModifySRAMAccess(bool enableSRAM, byte SRAMbank, bool waitForCompletion = true);
    bool LinkCommand_RunSecondaryPayload(byte payload[], int payloadLength, bool waitForCompletion = true);
    bool LinkCommand_ReadMemorySection(u32 dataPointer, byte outArray[], int outArraySize, bool waitForCompletion = true);

    // Some operations are too long to be done within the IRQ.
    // So we need to handle them in the main loop instead to avoid data corruption.
    void handleCartIO();

private:
    void loadPayload(GB_PayloadsFiles payload);
    void loadPayloadByROM(GameBoyROM rom);
    void loadCurrGameFromChecksum();
    bool allPacketsProcessed();
    bool processPacket();
    void consumePacket();
    bool prepareNextPacket();
    void waitForEnd();

    // Used for debug features
#define LINE_WIDTH 24
#define NUM_LINES 8
    char stuff[NUM_LINES][LINE_WIDTH];
    char line[LINE_WIDTH] = "OUT";
    unsigned link_cable_memory_section_index;
    unsigned link_cable_array_index;
    const u16 *debug_charset;
    unsigned writeBufferOffset;
};

extern LinkConnection globalLinkCable;
void linkCableIRQ();

#endif /* LINK_HANDLER_H_ */
