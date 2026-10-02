#ifndef _CHEADERWRITER_H
#define _CHEADERWRITER_H

#include "utils.h"

#define PATH_BUFFER_SIZE 4096

class CHeaderWriter
{
public:
    CHeaderWriter(const char *outPath, const char *filenameBase);
    ~CHeaderWriter();

    void addFileEntry(const char *path, const char *altName);
    bool isValid() const;

protected:
private:
    FILE *outFile_;
    char lineBuffer_[PATH_BUFFER_SIZE];
};

#endif