#include "utils.h"

#include <libgen.h>
#include <cstring>

#define PATH_BUFFER_SIZE 4096

static bool isCurrentCPULittleEndian();

const bool g_isLittleEndian = isCurrentCPULittleEndian();

// the attribute stuff is to make sure the compiler doesn't optimize this code away.
__attribute__((optimize("O0")))
static bool isCurrentCPULittleEndian()
{
	short int word = 0x0001;
   char *byte = (char *) &word;
   return (byte[0] ? true : false);
}

void extractFilenameAndExtension(const char *path, char *outFilename, char *outExtension, uint16_t extensionBufferSize)
{
    char pathBuffer[PATH_BUFFER_SIZE];
    strncpy(pathBuffer, path, PATH_BUFFER_SIZE);
    pathBuffer[PATH_BUFFER_SIZE - 1] = '\0';

    char *basenameStr = basename(pathBuffer);
    char *dot = strrchr(basenameStr, '.');
    // We don't want the extension in the chunk file names, so we terminate the string at the last dot if it exists.
    // If there is no dot, we just use the entire basename.
    if(dot)
    {
        *dot = '\0';
    }
    
    if(outExtension)
    {
      if(dot)
      {
         strncpy(outExtension, dot + 1, extensionBufferSize);
         outExtension[extensionBufferSize - 1] = '\0';
      }
      else if(extensionBufferSize > 0)
      {
         outExtension[0] = '\0';
      }
    }

    strncpy(outFilename, basenameStr, PATH_BUFFER_SIZE);
    outFilename[PATH_BUFFER_SIZE - 1] = '\0';
}