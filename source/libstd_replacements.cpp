#define NANOPRINTF_IMPLEMENTATION 1
// Remove what we don't need
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 0
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#include "libstd_replacements.h"
#include "libraries/nanoprintf/nanoprintf.h"
#include <cstdlib>
#include <stdarg.h>
#include <ctype.h>
#include "custom_malloc.h"

// recommended for a 32 bit system to have at least 33 bytes available
// source: https://cplusplus.com/reference/cstdlib/itoa/
// unfortunately itoa is not standardized, so we'll have to make do with snprintf
static char conversion_buffer[33];

const char* ptgb::to_string(int intVal)
{
    npf_snprintf(conversion_buffer, sizeof(conversion_buffer), "%d", intVal);
    return conversion_buffer;
}

const char* ptgb::to_string(unsigned int wordVal)
{
    npf_snprintf(conversion_buffer, sizeof(conversion_buffer), "%u", wordVal);
    return conversion_buffer;
}

// when compiling with -nostdlib++, we need to provide our own operator new and delete implementations
// Regular operator new
void* operator new(std::size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        // mimic standard behavior: throw std::bad_alloc
        // but we can't use std::bad_alloc without libstdc++
        // so instead we can abort or return nullptr
        // You can also implement a custom exception if needed
        return NULL;
    }
    return ptr;
}

// nothrow version
void* operator new(std::size_t size, const std::nothrow_t&) noexcept {
    return malloc(size);
}

// regular array operator new
void* operator new[](std::size_t size) {
    void* ptr = malloc(size);
    if (!ptr) {
        return NULL;
    }
    return ptr;
}

// nothrow array operator new
void* operator new[](std::size_t size, const std::nothrow_t&) noexcept {
    return malloc(size);
}

// operator delete
void operator delete(void* ptr) noexcept {
    free(ptr);
}

// array operator delete
void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// nothrow delete
void operator delete(void* ptr, const std::nothrow_t&) noexcept {
    free(ptr);
}

// nothrow array delete
void operator delete[](void* ptr, const std::nothrow_t&) noexcept {
    free(ptr);
}

// sized delete
void operator delete(void* ptr, std::size_t size) noexcept {
    (void)size;
    free(ptr);
}

// sized array delete
void operator delete[](void* ptr, std::size_t size) noexcept {
    (void)size;
    free(ptr);
}