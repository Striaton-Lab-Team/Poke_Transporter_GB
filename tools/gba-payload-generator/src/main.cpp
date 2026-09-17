#include "mystery_gift_builder.h"
#include "UncompressedFileContainerReader.h"
#include "rom_values/gba_rom_values.h"

#include <cstdio>
#include <cstring>
#include <utility>
#include <cstdlib>
#include <climits>

#define FLASH_SECTOR_SIZE 4096

static const uint16_t gen3CharsetEng[256]{
    0x20, 0xC0, 0xC1, 0xC2, 0xC7,  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0x20, 0xCE,
    0xCF, 0xD2, 0xD3, 0xD4, 0x152, 0xD9, 0xDA, 0xDB, 0xD1, 0xDF, 0xE0, 0xE1,
    0x20, 0xE7, 0xE8, 0xE9, 0xEA,  0xEB, 0xEC, 0x20, 0xEE, 0xEF, 0xF2, 0xF3,
    0xF4, 0x153, 0xF9, 0xFA, 0xFB, 0xF1, 0xBA, 0xAA, 0x1D49, 0x26, 0x2B, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x3D, 0x3B, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x25AF, 0xBF, 0xA1, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xCD, 0x25, 0x28, 0x29, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xE2, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0xED, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x2B07, 0x2B05, 0x27A1, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A,
    0x2A, 0x1D49, 0x3C, 0x3E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x2B3, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35,
    0x36, 0x37, 0x38, 0x39, 0x21, 0x3F, 0x2E, 0x2D, 0x30FB, 0x2026, 0x201C,
    0x201D, 0x2018, 0x2019, 0x2642, 0x2640, 0x20, 0x2C, 0xD7, 0x2F, 0x41,
    0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5A, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B,
    0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
    0x78, 0x79, 0x7A, 0x25B6, 0x3A, 0xC4, 0xD6, 0xDC, 0xE4, 0xF6, 0xFC, 0x20,
    0x20, 0x20, 0x15E, 0x23C, 0x206, 0x1B2, 0x147, 0x19E
};

static const std::pair<uint32_t, const char *> romIdStringMap[] = {
    {RUBY_ID, "ruby"},
    {SAPPHIRE_ID, "sapphire"},
    {FIRERED_ID, "firered"},
    {LEAFGREEN_ID, "leafgreen"},
    {EMERALD_ID, "emerald"}
};

static const std::pair<uint32_t, const char *> romVersionStringMap[] = {
    { VERS_1_0, "1_0" },
    { VERS_1_1, "1_1" },
    { VERS_1_2, "1_2" }
};

static const std::pair<char, const char *> romLanguageStringMap[] = {
    { LANG_JPN, "japanese" },
    { LANG_ENG, "english" },
    { LANG_FRE, "french" },
    { LANG_GER, "german" },
    { LANG_ITA, "italian" },
    { LANG_SPA, "spanish" }
};

/**
 * @brief Template utility function to look up a string conversion from a pair list.
 */
template <typename T>
static const char* convertXToString(T listKey, const std::pair<T, const char *> *list, size_t listSize)
{
    for (size_t i = 0; i < listSize; ++i)
    {
        if (list[i].first == listKey)
        {
            return list[i].second;
        }
    }
    return "unknown";
}

static const char* convertGameIdToString(uint32_t romId)
{
    const size_t numRoms = sizeof(romIdStringMap) / sizeof(romIdStringMap[0]);
    return convertXToString(romId, romIdStringMap, numRoms);
}

static const char* convertRomVersionToString(uint32_t romVersion)
{
    const size_t numVersions = sizeof(romVersionStringMap) / sizeof(romVersionStringMap[0]);
    return convertXToString(romVersion, romVersionStringMap, numVersions);
}

static const char* convertRomLanguageToString(char romLanguage)
{
    const size_t numLanguages = sizeof(romLanguageStringMap) / sizeof(romLanguageStringMap[0]);
    return convertXToString(romLanguage, romLanguageStringMap, numLanguages);
}

static void pickGBARomDataArray(char languageCode, const ROM_DATA *&gbaRomDataArray, uint16_t &gbaRomDataArraySize)
{
    switch (languageCode)
    {
        case LANG_JPN:
            gbaRomDataArray = rom_data_values_jpn;
            gbaRomDataArraySize = rom_data_values_jpn_size;
            break;
        case LANG_ENG:
            gbaRomDataArray = rom_data_values_eng;
            gbaRomDataArraySize = rom_data_values_eng_size;
            break;
        case LANG_FRE:
            gbaRomDataArray = rom_data_values_fre;
            gbaRomDataArraySize = rom_data_values_fre_size;
            break;
        case LANG_GER:
            gbaRomDataArray = rom_data_values_ger;
            gbaRomDataArraySize = rom_data_values_ger_size;
            break;
        case LANG_ITA:
            gbaRomDataArray = rom_data_values_ita;
            gbaRomDataArraySize = rom_data_values_ita_size;
            break;
        case LANG_SPA:
            gbaRomDataArray = rom_data_values_spa;
            gbaRomDataArraySize = rom_data_values_spa_size;
            break;
        default:
            fprintf(stderr, "Error: Unsupported language code %c!\n", languageCode);
            exit(1);
    }
}

static void generateOutputPath(char *outputPathBuffer, const char *outputDir, const char* baseString, const ROM_DATA *romData)
{
    const char *gameName = convertGameIdToString(romData->gamecode);
    const char *versionString = convertRomVersionToString(romData->version);
    const char *languageString = convertRomLanguageToString(static_cast<char>(romData->language));

    snprintf(outputPathBuffer, PATH_MAX, "%s/%s_%s_%s_%s.bin", outputDir, baseString, gameName, languageString, versionString);
}

static void printUsage()
{
    printf("Usage: mystery_gift_builder path/to/RSEFRLG_text_table.bin outputPath\n");
    printf("  path/to/RSEFRLG_text_table.bin: Path to the input RSEFRLG text table file.\n");
    printf("  outputPath: Path to a directory to output our language-specific payloads to.\n\n");
    printf("gba-payload-generator will generate all payloads for the specified language.\n");
}

static void generatePayloadsForLanguage(const char* outputPath, char languageCode, u8 *textTableBuffer, uint32_t textTableSize)
{
    u8 section30Buffer[FLASH_SECTOR_SIZE];
    u8 mgScriptBuffer[MG_SCRIPT_SIZE];
    char outputPathBuffer[PATH_MAX];
    const ROM_DATA *gbaRomDataArray;
    uint16_t gbaRomDataArraySize;

    pickGBARomDataArray(languageCode, gbaRomDataArray, gbaRomDataArraySize);
    
    // now we can start the real work.
    const u8 *chunkList[] = { textTableBuffer };
    UncompressedFileContainerReader rsefrlgTableReader(chunkList, 1, textTableSize);
    mystery_gift_script builder(section30Buffer, mgScriptBuffer);

    rsefrlgTableReader.init();

    for(size_t i = 0; i < gbaRomDataArraySize; ++i)
    {
        memset(section30Buffer, 0, FLASH_SECTOR_SIZE);
        memset(mgScriptBuffer, 0, MG_SCRIPT_SIZE);

        generateOutputPath(outputPathBuffer, outputPath, "section30", gbaRomDataArray + i);

        printf("[gba-payload-generator]: Generating payload: %s\n", outputPathBuffer);
        builder.build_script(rsefrlgTableReader, gbaRomDataArray[i], gen3CharsetEng, nullptr, true);

        // strip the injected texts again for compression sake.
        // doing it this way preserves the pointers (because the text had been temporarily injected)
        // but it does remove the potential for storing texts more than once in the patch files
        // because of the FileDataContainer chunks.
        builder.strip_injected_texts();

        FILE *section30OutputFile = fopen(outputPathBuffer, "wb");
        if (!section30OutputFile)
        {
            fprintf(stderr, "[gba-payload-generator]: Error: Could not open output file %s for writing! Skipping!\n", outputPathBuffer);
            continue;
        }

        size_t write_size = fwrite(section30Buffer, 1, builder.get_section30_size(), section30OutputFile);
        if (write_size != builder.get_section30_size())
        {
            fprintf(stderr, "[gba-payload-generator]: Error: Could not write to output file %s!\n", outputPathBuffer);
        }
        fclose(section30OutputFile);

        generateOutputPath(outputPathBuffer, outputPath, "script", gbaRomDataArray + i);
        FILE *mgScriptOutputFile = fopen(outputPathBuffer, "wb");
        if (!mgScriptOutputFile)
        {
            fprintf(stderr, "[gba-payload-generator]: Error: Could not open output file %s for writing! Skipping!\n", outputPathBuffer);
            continue;
        }

        write_size = fwrite(mgScriptBuffer, 1, builder.get_script_size(), mgScriptOutputFile);
        if (write_size != builder.get_script_size())
        {
            fprintf(stderr, "[gba-payload-generator]: Error: Could not write to output file %s!\n", outputPathBuffer);
        }
        fclose(mgScriptOutputFile);
    }
}

int main(int argc, char **argv)
{
    u8 *textTableBuffer = nullptr;
    uint32_t textTableSize = 0;
    char languageCode;

    if(argc != 3)
    {
        printUsage();
        return 1;
    }

    // set up dependencies.
    FILE *text_table_file = fopen(argv[1], "rb");
    if (!text_table_file)
    {
        fprintf(stderr, "[gba-payload-generator]: Error: Could not open RSEFRLG text table file %s!\n", argv[1]);
        printUsage();
        return 1;
    }

    // determine file size and allocate buffer
    fseek(text_table_file, 0, SEEK_END);
    textTableSize = ftell(text_table_file);
    fseek(text_table_file, 0, SEEK_SET);
    textTableBuffer = new u8[textTableSize];

    // read RSEFRLG text table into buffer
    const size_t read_size = fread(textTableBuffer, 1, textTableSize, text_table_file);
    if (read_size != textTableSize)
    {
        fprintf(stderr, "[gba-payload-generator]: Error: Could not read RSEFRLG text table file %s!\n", argv[1]);
        printUsage();

        delete[] textTableBuffer;
        textTableBuffer = nullptr;

        fclose(text_table_file);
        return 1;
    }
    fclose(text_table_file);

    const char langCodes[] = { LANG_ENG, LANG_FRE, LANG_SPA, LANG_ITA, LANG_GER, LANG_JPN };

    for(char langCode : langCodes)
    {
        generatePayloadsForLanguage(argv[2], langCode, textTableBuffer, textTableSize);
    }

    delete[] textTableBuffer;
    textTableBuffer = nullptr;

    return 0;
}