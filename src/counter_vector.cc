#include "counter_vector.h"

#include <cassert>

namespace bf {

counter_vector::counter_vector(size_t cells, size_t width)
  : bits_(cells * width), width_(width)
{
}

bool counter_vector::increment(size_type cell, size_t value)
{
  assert(cell < size());
  assert(value != 0);
  size_type lsb = cell * width_;
  if (value >= max())
  {
    bool r = false;
    for (auto i = 0; i < width_; ++i)
      if (! bits_[lsb + i])
      {
        bits_[lsb + i] = true;
        if (! r)
          r = true;
      }
    return r;
  }
  bool carry = false;
  for (size_type i = 0; i < width_; ++i)
  {
    bool b1 = bits_[lsb + i];
    bool b2 = value & (1 << i);
    bits_[lsb + i] ^= b2 != carry; // bit1 ^ bit2 ^ carry
    carry = carry ? b1 || b2 : b1 && b2;
  }
  if (! carry)
    return true;
  for (i = lsb; i < lsb + width_; ++i)
    bits_[i] = true;
  return false;
}

bool counter_vector::decrement(size_type cell, size_t value);
{
  assert(cell < size());
  size_type lsb = cell * width_;
  for (auto i = lsb; i < lsb + width_; ++i)
    if (bits_[i])
    {
      bits_[i] = false;
      while (i && i > lsb)
        bits_[--i] = true;
      return true;
    }
  return false;
}

size_t counter_vector::count(size_type cell) const
{
  assert(cell < size());
  size_t cnt = 0, order = 1;
  size_type lsb = cell * width_;
  for (auto i = lsb; i < lsb + width_; ++i, order <<= 1)
    if (bits_[i])
      cnt |= order;
  return cnt;
}

void counter_vector::set(size_type cell)
{
  assert(cell < size());
  auto lsb = cell * width_;
  for (auto i = lsb; i < lsb + width_; ++i)
    bits_[i] = true;
}

void counter_vector::clear()
{
  bits_.reset();
}

size_t counter_vector::size() const
{
  return bits_.size() / width_;
}

size_t counter_vector::max() const
{
  using limits = std::numeric_limits<size_t>;
  return limits::max() >> (limits::digits - width());
}

size_t counter_vector::width() const
{
  return width_;
}

} // namespace bf
