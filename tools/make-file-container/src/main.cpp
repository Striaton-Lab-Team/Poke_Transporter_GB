#include "FileContainerChunkWriter.h"
#include "CHeaderWriter.h"

#include <cstdio>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <libgen.h>
#include <getopt.h>

#include <sys/stat.h>

#define FILE_RECORD_NAME_LENGTH 16
#define LINE_BUFFER_SIZE 2048
#define PATH_BUFFER_SIZE 4096
#define DEFAULT_CHUNK_SIZE 2048


typedef struct
{
    uint16_t size;
    char altName[FILE_RECORD_NAME_LENGTH];
    char path[PATH_BUFFER_SIZE];
    bool isDummy;
} FileRecord;

typedef struct
{
    uint8_t hasNames;
    /**
     * @brief Indicates whether the container can span a single file across multiple chunks.
     * If set to 1, it won't.
     * This is useful to ensure that we can safely use FileContainerReader::getPointerToFileInDecompressionBuffer() in PTGB, 
     * which assumes that files are not split across chunks.
     */
    uint8_t noSpanChunks;
    uint16_t chunkSize;
    std::vector<FileRecord> entries;
} ContainerMetadata;

static void printUsage()
{
    printf("Usage: make-file-container [-n] [-H <header output folder>] <output folder> <path/to/file.containerdef>\n\n");
    printf("Definition syntax:\n  <filename>[:<optional alternative name>]\n\n");
    printf("Flags:\n  -n    Store filenames in the container\n");
    printf("  -H, --header-out <path>    Write generated header to this folder\n");
}

/**
 * @brief This function parses a directive.
 * A directive starts with '@' and is used to set certain parameters for the container generation.
 * Right now, we only support @chunkSize and @noSpanChunks.
 */
static void parseDirective(ContainerMetadata &meta, char *line)
{
    char *equals;
    if(strncmp(line + 1, "chunkSize", 9) == 0)
    {
        char *endptr;
        equals = strchr(line, '=');
        if(equals == NULL)
        {
            fprintf(stderr, "Invalid directive (missing '='): %s\n", line);
            return;
        }
        const long value = strtol(equals + 1, &endptr, 10);
        if(*endptr != '\0' || value <= 0 || value > 0xFFFF)
        {
            fprintf(stderr, "Invalid chunk size value: %s\n", equals + 1);

            return;
        }
        meta.chunkSize = static_cast<uint16_t>(value);
    }
    else if(strncmp(line + 1, "noSpanChunks", 12) == 0)
    {
        meta.noSpanChunks = 1;
    }
    else
    {
        fprintf(stderr, "Unknown directive: %s\n", line);
    }
}

/**
 * @brief This function parses a single line from the definition file, extracting the file path and optional alternative name.
 */
static bool parseDefinitionLine(char *line, char **path, char **altName)
{
    char *colon = strchr(line, ':');

    if(colon != NULL)
{
        *colon = '\0';
        *altName = colon + 1;
        if((*altName)[0] == '\0')
        {
            *altName = NULL;
        }
    }
    else
    {
        *altName = NULL;
    }

    *path = line;
    return (*path)[0] != '\0';
}

/**
 * @brief This function determines the file size
 */
static bool determineFileSize(const char *path, uint16_t &outSize)
{
    struct stat st;

    if(stat(path, &st) != 0)
    {
        fprintf(stderr, "Failed to stat input file: %s\n", path);
        return false;
    }

    if(!S_ISREG(st.st_mode))
    {
        fprintf(stderr, "Input path is not a regular file: %s\n", path);
        return false;
    }

    if(st.st_size < 0 || st.st_size > 0xFFFF)
    {
        fprintf(stderr, "Input file too large for uint16 size field: %s\n", path);
        return false;
    }

    outSize = static_cast<uint16_t>(st.st_size);
    return true;
}

/**
 * @brief This function parses the container definition file.
 */
static bool parseDefinition(const char *defPath, ContainerMetadata &meta)
{
    FILE *f = fopen(defPath, "rb");
    char line[LINE_BUFFER_SIZE];
    char *filePath;
    char *altName;
    size_t line_no = 0;

    if(f == NULL)
    {
        fprintf(stderr, "Failed to open container definition: %s\n", defPath);
        return false;
    }

    while (fgets(line, sizeof(line), f) != NULL)
    {
        line[strcspn(line, "\r\n")] = '\0';
        FileRecord entry;

        ++line_no;
        if(line[0] == '\0' || line[0] == '#')
        {
            continue;
        }

        if(line[0] == '@')
        {
            parseDirective(meta, line);
            continue;
        }

        if(!parseDefinitionLine(line, &filePath, &altName))
        {
            fprintf(stderr, "Invalid entry at line %zu\n", line_no);
            fclose(f);
            return false;
        }

        memset(&entry, 0, sizeof(entry));

        if(!determineFileSize(filePath, entry.size))
        {
            fclose(f);
            return false;
        }

        strncpy(entry.path, filePath, PATH_BUFFER_SIZE - 1);
        entry.path[PATH_BUFFER_SIZE - 1] = '\0';

        if(altName != NULL)
        {
            strncpy(entry.altName, altName, FILE_RECORD_NAME_LENGTH - 1);
            entry.altName[FILE_RECORD_NAME_LENGTH - 1] = '\0';
        }
        entry.isDummy = false;

        meta.entries.push_back(entry);
    }

    fclose(f);

    if(meta.entries.empty())
    {
        fprintf(stderr, "No files defined in: %s\n", defPath);
        return false;
    }

    return true;
}

static void guardAgainstFileSpanningChunks(ContainerMetadata &meta)
{
    if(!meta.noSpanChunks)
    {
        return;
    }
    uint16_t remainingChunkSize = meta.chunkSize;
    unsigned dummyNum = 0;

    for(auto it = meta.entries.begin(); it != meta.entries.end(); ++it)
    {
        if(it->size <= remainingChunkSize)
        {
            remainingChunkSize -= it->size;
            if(remainingChunkSize == 0)
            {
                remainingChunkSize = meta.chunkSize;
            }
        }
        else
        {
            if(it->size > meta.chunkSize)
            {
                fprintf(stderr, "File %s is too large to fit in a single chunk (size: %u, chunk size: %u)\n", it->path, it->size, meta.chunkSize);
                exit(EXIT_FAILURE);
            }
            // insert dummy record
            FileRecord dummyEntry = {
                .size = remainingChunkSize,
                .altName = {0},
                .path = {0},
                .isDummy = true
            };

            snprintf(dummyEntry.path, PATH_BUFFER_SIZE, "DUMMY_%u", dummyNum);
            ++dummyNum;

            it = meta.entries.insert(it, dummyEntry);
            remainingChunkSize = meta.chunkSize;

        }
    }
}

/**
 * @brief This function reads a file into memory, allocating a buffer and returning its size. The caller is responsible for freeing the buffer.
 */
static void readFileIntoMemory(const char *path, uint8_t **outBuffer, uint16_t &outSize)
{
    FILE *f = fopen(path, "rb");
    size_t read_size;

    if(f == NULL)
    {
        fprintf(stderr, "Failed to open input file: %s\n", path);
        return;
    }

    if(!determineFileSize(path, outSize))
    {
        fclose(f);
        return;
    }

    *outBuffer = static_cast<uint8_t *>(std::malloc(outSize));

    read_size = fread(*outBuffer, 1, outSize, f);
    fclose(f);

    if(read_size != outSize)
    {
        fprintf(stderr, "Failed to read entire file into memory: %s\n", path);
        free(*outBuffer);
        *outBuffer = NULL;
        return;
    }
}

static void writeHeader(FileContainerChunkWriter &writer, const ContainerMetadata &meta)
{

    const uint8_t num_files = static_cast<uint8_t>(meta.entries.size());

    writer.writeUint8(num_files);
    writer.writeUint8(meta.hasNames);
    writer.writeUint16(meta.chunkSize);
}

/**
 * @brief This function writes the file index.
 * It just stores the file sizes for every entry, not the offsets.
 * The reason is two-fold:
 *
 * - It makes it much better for compression, as file sizes will repeat more often than accumulating offsets
 * - If you'd use offsets and base your file size calculation on them, you'd get into trouble with the last entry.
 */
static void writeIndex(FileContainerChunkWriter &writer, ContainerMetadata &meta)
{
    for(auto &entry : meta.entries)
    {
        writer.writeUint16(entry.size);
    }
}

static void writeNames(FileContainerChunkWriter &writer, const ContainerMetadata &meta)
{
    char nameBuffer[FILE_RECORD_NAME_LENGTH];
    char pathBasename[PATH_BUFFER_SIZE];

    for(auto &entry : meta.entries)
    {
        strncpy(pathBasename, entry.path, PATH_BUFFER_SIZE - 1);
        pathBasename[PATH_BUFFER_SIZE - 1] = '\0';
        // Not using FILE_RECORD_NAME_LENGTH - 1 is intentional here. I want to be able to use the last character.
        // When reading the data, we just have to allocate an extra byte and set it to 0 to ensure null termination, so we can safely use the entire buffer for the name.
        strncpy(nameBuffer, entry.altName[0] != '\0' ? entry.altName : basename(pathBasename), FILE_RECORD_NAME_LENGTH);
        writer.write(reinterpret_cast<uint8_t*>(nameBuffer), FILE_RECORD_NAME_LENGTH);
    }
}

static void writeFiles(FileContainerChunkWriter &writer, ContainerMetadata &meta)
{
    uint8_t *fileBuffer = NULL;
    uint16_t fileSize;

    for(auto &entry : meta.entries)
    {
        if(entry.isDummy)
        {
            // this is a dummy entry
            fileSize = entry.size;
            fileBuffer = (uint8_t*)calloc(1, fileSize);
        }
        else
        {
            readFileIntoMemory(entry.path, &fileBuffer, fileSize);
        }

        if(!fileBuffer)
        {
            fprintf(stderr, "Failed to read file into memory: %s\n", entry.path);
            return;
        }

        writer.write(fileBuffer, fileSize);
        free(fileBuffer);
    }
}

static bool writeContainer(ContainerMetadata &meta, const char *outPath, const char *containerDefPath)
{
    char nameBuffer[PATH_BUFFER_SIZE];
    extractFilenameAndExtension(containerDefPath, nameBuffer, nullptr);

    FileContainerChunkWriter writer(outPath, nameBuffer, meta.chunkSize);

    writeHeader(writer, meta);
    writeIndex(writer, meta);

    if(meta.hasNames)
    {
        writeNames(writer, meta);
    }

    writeFiles(writer, meta);

    return true;
}

static bool writeHeaderFile(const ContainerMetadata &meta, const char *headerOutPath, const char *containerDefPath)
{
    char nameBuffer[PATH_BUFFER_SIZE];
    extractFilenameAndExtension(containerDefPath, nameBuffer, nullptr);

    CHeaderWriter headerWriter(headerOutPath, nameBuffer);
    if(!headerWriter.isValid())
    {
        return false;
    }

    for(const auto &entry : meta.entries)
    {
        headerWriter.addFileEntry(entry.path, entry.altName);
    }

    return true;
}

static bool parseArgs(int argc, char **argv, bool *storeNames, const char **headerOutPath, const char **defPath, const char **outPath)
{
    int opt;
    int option_index = 0;
    static const struct option long_options[] = {
        {"header-out", required_argument, 0, 'H'},
        {0, 0, 0, 0}
    };

    *storeNames = false;
    *headerOutPath = NULL;

    opterr = 0;
    optind = 1;

    while((opt = getopt_long(argc, argv, "nH:", long_options, &option_index)) != -1)
    {
        switch(opt)
        {
        case 'n':
            *storeNames = true;
            break;
        case 'H':
            *headerOutPath = optarg;
            break;
        default:
            return false;
        }
    }

    if(optind + 2 != argc)
    {
        return false;
    }

    *outPath = argv[optind];
    *defPath = argv[optind + 1];

    return true;
}

int main(int argc, char *argv[])
{
    ContainerMetadata meta;
    bool storeNames = false;
    const char *headerOutPath = NULL;
    const char *defPath = NULL;
    const char *outPath = NULL;
    int rc = 1;

    if(!parseArgs(argc, argv, &storeNames, &headerOutPath, &defPath, &outPath))
    {
        printUsage();
        return rc;
    }

    meta.hasNames = storeNames ? 1 : 0;
    meta.noSpanChunks = 0;
    meta.chunkSize = DEFAULT_CHUNK_SIZE;

    if(!parseDefinition(defPath, meta))
    {
        return rc;
    }
    guardAgainstFileSpanningChunks(meta);

    if(meta.entries.size() >= 0xFF)
    {
        fprintf(stderr, "Too many files (max 255).\n");
        exit(1);
    }

    if(headerOutPath != NULL)
    {
        if(!writeHeaderFile(meta, headerOutPath, defPath))
        {
            fprintf(stderr, "Failed to write output header to: %s\n", headerOutPath);
            return rc;
        }
    }

    if(!writeContainer(meta, outPath, defPath))
    {
        fprintf(stderr, "Failed to write output container: %s\n", outPath);
    }
    else
    {
        printf("Built container: files=%zu, hasNames=%u\n", meta.entries.size(), storeNames ? 1u : 0u);
        if(headerOutPath != NULL)
        {
            printf("Built header in: %s\n", headerOutPath);
        }
        rc = 0;
    }

    return rc;
}
