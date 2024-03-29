#ifndef ENCODED_PATH_H__
#define ENCODED_PATH_H__

#include <string.h>
#include <fmt/core.h>

namespace boxedin
{

//FIXME: Do not use size, at, push_back function names in EncodePath class.
//       This isn't a std container.
//FIXME: move to config.h and bump it up. there are known optimal solutions
//       around 256 moves.
//TODO: can you #pragma pack to reduce the size of Node and it's contained
//      types?
#define ENCODED_PATH_LENGTH_MAX 256
#define ENCODED_PATH_DATA_SIZE (ENCODED_PATH_LENGTH_MAX / 4)
enum EncodedPathDirection // TODO: change to enum class
{
    ENCODED_PATH_DIRECTION_UP = 0,
    ENCODED_PATH_DIRECTION_RIGHT,
    ENCODED_PATH_DIRECTION_DOWN,
    ENCODED_PATH_DIRECTION_LEFT,
};

inline EncodedPathDirection CharToDirection(char c)
{
  switch (c)
  {
    case 'U':
      return ENCODED_PATH_DIRECTION_UP;
    case 'D':
      return ENCODED_PATH_DIRECTION_DOWN;
    case 'L':
      return ENCODED_PATH_DIRECTION_LEFT;
    case 'R':
      return ENCODED_PATH_DIRECTION_RIGHT;
    default:
      // FIXME: throw custom exception type
      throw std::runtime_error(fmt::format("Invalid direction char: {}", c));
    }
}

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
    EncodedPath(const std::string& pathStr)
        : size_(0)
    {
        memset(data_, 0, ENCODED_PATH_DATA_SIZE);
        for (auto c : pathStr)
        {
            push_back(CharToDirection(c));
        }
    }
    uint8_t size() const { return size_; }
    EncodedPathDirection at(int i) const
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
            // fmt::print("data[{}]={:#010b}\n", data_index, data_[data_index]);
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
    // FIXME: could be optimized, but this is only used for unit tests to sort list<Action>
    bool operator<(const EncodedPath& other) const
    {
        if (size() != other.size())
            return size() < other.size();
        int res = memcmp(data_, other.data_, size());
        if (res != 0)
            return res < 0;
        return false;
    }
    bool operator==(const EncodedPath& other) const
    {
        if (size() != other.size())
            return false;
        if (memcmp(data_, other.data_, size()) != 0)
            return false;
        return true;
    }
private:
    uint8_t size_;
    uint8_t data_[ENCODED_PATH_DATA_SIZE];
};


} // namespace boxedin

#endif
