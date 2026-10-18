#ifndef _LIBSTD_REPLACEMENTS_H
#define _LIBSTD_REPLACEMENTS_H

#include <cstring>
#include <inttypes.h>
#include <new>
#include <utility>

// To reduce the binary size, we need to get rid of libstdc++
// But we were happily using some functions that made life easier.
// so this file was created to provide some similar functions/functionalities
// (although stripped down to the bare minimum we're using)

namespace ptgb
{
/**
 * This utility/convenience function converts the given value to a string.
 * WARNING: it AND its other variants always use the same buffer.
 * So calling it more than once will overwrite the previously returned string!
 *
 * Be careful!
 * In case you want to convert more than one int to string within the same call,
 * consider using npf_snprintf instead!
 *
 * Also: if you want to hold on to the converted value, consider copying the
 * returned string
 */
const char *to_string(int intVal);

/**
 * This utility/convenience function converts the given value to a string.
 * WARNING: it AND its other variants always use the same buffer.
 * So calling it more than once will overwrite the previously returned string!
 *
 * Be careful!
 * In case you want to convert more than one int to string within the same call,
 * consider using npf_snprintf instead!
 *
 * Also: if you want to hold on to the converted value, consider copying the
 * returned string
 */
const char *to_string(unsigned int wordVal);

template <class ValueType> class vector
{
  public:
    static constexpr uint16_t default_capacity = 2;

    vector() : buffer_(), capacity_(default_capacity), count_(0)
    {
        buffer_ = static_cast<ValueType *>(
            ::operator new(sizeof(ValueType) * capacity_));
        if (!buffer_)
        {
            return; // allocation failed
        }
    }

    vector(const ValueType *valueList, uint16_t listSize)
        : buffer_(), capacity_(listSize), count_(0)
    {
        buffer_ = static_cast<ValueType *>(
            ::operator new(sizeof(ValueType) * capacity_));
        if (!buffer_)
        {
            return; // allocation failed
        }
        insert(valueList, listSize);
    }

    vector(const vector<ValueType> &otherList)
        : buffer_(), capacity_(otherList.capacity_), count_(0)
    {
        buffer_ = static_cast<ValueType *>(
            ::operator new(sizeof(ValueType) * capacity_));
        if (!buffer_)
        {
            return; // allocation failed
        }
        insert(otherList);
    }

    ~vector()
    {
        clear();
        ::operator delete(buffer_);
    }

    void reserve(uint16_t newSize)
    {
        if (newSize <= capacity_)
        {
            return;
        }
        ValueType *new_buffer = static_cast<ValueType *>(
            ::operator new(sizeof(ValueType) * newSize));
        if (!new_buffer)
        {
            return; // allocation failed, keep the old buffer
        }

        // copy data from the old buffer to the new one
        for (uint16_t i = 0; i < count_; ++i)
        {
            new (new_buffer + i) ValueType(std::move(buffer_[i]));
            buffer_[i].~ValueType();
        }

        ::operator delete(buffer_);

        buffer_ = new_buffer;
        capacity_ = newSize;
    }

    void resize(uint16_t newSize) { resize(newSize, ValueType()); }

    void resize(uint16_t newSize, const ValueType &value)
    {
        if (newSize < count_)
        {
            const uint16_t num_erase = (count_ - newSize);
            for (uint16_t i = 0; i < num_erase; ++i)
            {
                pop_back();
            }
        }
        else if (newSize > count_)
        {
            reserve(newSize);
            const uint16_t num_fill = (newSize - count_);
            for (uint16_t i = 0; i < num_fill; ++i)
            {
                push_back(value);
            }
        }
    }

    void push_back(const ValueType &value)
    {
        if (count_ == capacity_)
        {
            reserve(capacity_ * 2);
        }
        new (buffer_ + count_) ValueType(value);
        ++count_;
    }

    void pop_back()
    {
        if (count_ == 0)
            return;
        ValueType *value = buffer_ + (count_ - 1);
        value->~ValueType();
        --count_;
    }

    void insert(const vector<ValueType> &otherList)
    {
        reserve(count_ + otherList.size());

        for (uint16_t i = 0; i < otherList.size(); ++i)
        {
            push_back(otherList.at(i));
        }
    }

    void insert(const ValueType *list, uint16_t listSize)
    {
        reserve(count_ + listSize);
        for (size_t i = 0; i < listSize; ++i)
        {
            push_back(list[i]);
        }
    }

    void clear() { resize(0); }

    uint16_t size() const { return count_; }
    uint16_t capacity() const { return capacity_; }

    ValueType *data() const { return buffer_; }

    ValueType *data() { return buffer_; }

    ValueType &at(uint16_t index) { return operator[](index); }

    const ValueType &at(uint16_t index) const { return operator[](index); }

    ValueType &operator[](uint16_t index) { return *(buffer_ + index); }

    const ValueType &operator[](uint16_t index) const
    {
        return *(buffer_ + index);
    }

  private:
    ValueType *buffer_;
    uint16_t capacity_;
    uint16_t count_;
};
} // namespace ptgb

#endif