#ifndef ENCODED_PATH_H__
#define ENCODED_PATH_H__

#include <string.h>


namespace boxedin
{
    
#define ENCODED_PATH_LENGTH_MAX 256
#define ENCODED_PATH_DATA_SIZE (ENCODED_PATH_LENGTH_MAX / 4)
enum EncodedPathDirection // TODO: change to enum class
{
    ENCODED_PATH_DIRECTION_UP = 0,
    ENCODED_PATH_DIRECTION_RIGHT,
    ENCODED_PATH_DIRECTION_DOWN,
    ENCODED_PATH_DIRECTION_LEFT,
};
class EncodedPath
{
public:
    EncodedPath()
        : size_(0)
    {
        memset(data_, 0, ENCODED_PATH_DATA_SIZE);
    }
    EncodedPath(const EncodedPath& other)
        : size_(other.size_)
    {
        memcpy(data_, other.data_, ENCODED_PATH_DATA_SIZE);
    }
    uint8_t size() const { return size_; }
    EncodedPathDirection at(int i) const // TODO: why not overload operator[] too?
    {
        uint8_t data_index = (uint8_t)i / 4;
        int shift = i % 4;
        uint8_t direction = ((data_[data_index] >> (shift * 2)) & 0x3);
        return (EncodedPathDirection)direction;
    }
    void push_back(EncodedPathDirection direction)
    {
        if (size_ < ENCODED_PATH_DATA_SIZE)
        {
            uint8_t data_index = (uint8_t)size_ / 4;
            int shift = size_ % 4;
            data_[data_index] |= ((uint8_t)direction << (shift * 2));
            size_++;
        }
        else
        {
            // TODO: throw exception instead of printing error?
            fprintf(stderr, "error: encoded path is already maximum size\n");
        }
    }
    EncodedPath& operator=(const EncodedPath& other)
    {
        size_ = other.size_;
        memcpy(data_, other.data_, ENCODED_PATH_DATA_SIZE);
        return *this;
    }
private:
    uint8_t size_;
    uint8_t data_[ENCODED_PATH_DATA_SIZE];
};


} // namespace boxedin

#endif
