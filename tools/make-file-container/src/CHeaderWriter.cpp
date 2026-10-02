#include "CHeaderWriter.h"

#include <cctype>
#include <cstdio>
#include <cstring>

#define PATH_BUFFER_SIZE 4096

static const char headerTemplatePrefix[] = R"delim(
#ifndef _%s_H
#define _%s_H

enum class %sFiles
{
)delim";

static const char headerTemplateSuffix[] = R"delim(
};
#endif
)delim";

static const char fileEntryTemplate[] = "\t%s,\n";

static void replaceWhiteSpaceWithUnderscore(const char *input, char *output)
{
    while(*input)
    {
        if(*input == ' ' || *input == '-' || *input == '.' || *input == '\t')
        {
            *output = '_';
        }
        else
        {
            *output = *input;
        }
        ++input;
        ++output;
    }
    *output = '\0';
}

static void convertToUpperCase(char *str, size_t length)
{
    for(size_t i = 0; i < length; ++i)
    {
        str[i] = toupper(str[i]);
    }
}

CHeaderWriter::CHeaderWriter(const char *outPath, const char *filenameBase)
    : outFile_(nullptr)
    , lineBuffer_{0}
{
    char headerPath[PATH_BUFFER_SIZE];
    snprintf(headerPath, PATH_BUFFER_SIZE, "%s/%s.h", outPath, filenameBase);
    outFile_ = fopen(headerPath, "w");
    if(!outFile_)
    {
        fprintf(stderr, "Failed to open output file: %s\n", headerPath);
        return;
    }

    char nameBuffer[4096];
    char nameBufferUppercase[4096];
    strncpy(nameBuffer, filenameBase, sizeof(nameBuffer));
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    // replace whitespace to underscores
    replaceWhiteSpaceWithUnderscore(nameBuffer, nameBuffer);
    // capitalize first letter
    convertToUpperCase(nameBuffer, 1);

    strncpy(nameBufferUppercase, nameBuffer, sizeof(nameBuffer));
    nameBufferUppercase[sizeof(nameBufferUppercase) - 1] = '\0';
    convertToUpperCase(nameBufferUppercase, strlen(nameBufferUppercase));

    fprintf(outFile_, headerTemplatePrefix, nameBufferUppercase, nameBufferUppercase, nameBuffer);
}

CHeaderWriter::~CHeaderWriter()
{
    if(outFile_)
    {
        fputs(headerTemplateSuffix, outFile_);
        fclose(outFile_);
    }
}

void CHeaderWriter::addFileEntry(const char *path, const char *altName)
{
    if(!outFile_)
    {
        return;
    }

    char nameBuffer[PATH_BUFFER_SIZE];
    char pathBasename[PATH_BUFFER_SIZE];
    extractFilenameAndExtension(path, pathBasename, nullptr);

    strncpy(nameBuffer, (altName && altName[0] != '\0') ? altName : pathBasename, sizeof(nameBuffer));
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';

    // replace whitespace to underscores
    replaceWhiteSpaceWithUnderscore(nameBuffer, nameBuffer);
    convertToUpperCase(nameBuffer, strlen(nameBuffer));

    fprintf(outFile_, fileEntryTemplate, nameBuffer);
}

bool CHeaderWriter::isValid() const
{
    return outFile_ != nullptr;
}