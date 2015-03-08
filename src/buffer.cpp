
#include "buffer.hpp"
#include <cstring>
#include <stdexcept>

const buffer::size_type buffer::npos = -1;

buffer::~buffer()
{
    delete[] _data;
}

buffer::buffer(size_type s):
_data(nullptr), _size(s), _capacity(s)
{
    if(s > 0)
    {
        _data = new value_type[s];
    }
}

buffer::buffer(size_type s, size_type c):
_data(nullptr), _size(s), _capacity(c)
{
    if (s > c)
    {
        throw std::invalid_argument("size greater than capacity");
    }
    if(c > 0)
    {
        _data = new value_type[c];
    }
}

buffer::buffer (const void* d, size_type s):
_data(nullptr), _size(s), _capacity(s)
{
    if (s != 0)
    {
        _data = new value_type[s];
        std::memcpy(_data, d, s);
    }
}

buffer::buffer (const void* d, size_type s, size_type c):
_data(nullptr), _size(s), _capacity(c)
{
    if (s > c)
    {
      throw std::invalid_argument("size greater than capacity");
    }

    if (c != 0)
    {
      _data = new value_type[c];
      if (s != 0)
      {
          std::memcpy(_data, d, s);
      }
    }
}

buffer::buffer(const buffer& x):
_data(nullptr), _size(x._size), _capacity(x._capacity)
{
    if (x._capacity != 0)
    {
        _data = new value_type[x._capacity];
    }
    if (x._size != 0)
    {
        std::memcpy(_data, x._data, x._size);
    }
}

buffer::buffer(buffer&& x):
_data(x._data), _size(x._size), _capacity(x._capacity)
{
    x._data = nullptr;
    x._size = 0;
    x._capacity = 0;
}

buffer& buffer::operator=(const buffer& x)
{
    if (&x != this)
    {
        if (x._size > _capacity)
        {
            delete[] _data;
            _data = new value_type[x._capacity];
            _capacity = x._capacity;
        }

        if (x._size != 0)
        {
            std::memcpy (_data, x._data, x._size);
        }
        _size = x._size;
    }

    return *this;
}

buffer& buffer::operator=(buffer&& x)
{
    if (&x != this)
    {
        delete[] _data;
        _data = x._data;
        _size = x._size;
        _capacity = x._capacity;
        x._data = nullptr;
        x._size = 0;
        x._capacity = 0;
    }

    return *this;
}

void buffer::swap(buffer& x)
{
    value_type* d(x._data);
    size_type s(x._size);
    size_type c(x._capacity);

    x._data = _data;
    x._size = _size;
    x._capacity = _capacity;

    _data = d;
    _size = s;
    _capacity = c;
}

buffer::value_type* buffer::detach()
{
    value_type* r(_data);
    _data = nullptr;
    _size = 0;
    _capacity = 0;

    return r;
}

void buffer::assign(const void* d, size_type s)
{
    if (s > _capacity)
    {
        delete[] _data;
        _data = new value_type[s];
        _capacity = s;
    }

    if (s != 0)
    {
        std::memcpy(_data, d, s);
    }
    _size = s;
}

void buffer::append(const buffer& x)
{
    append(x.data(), x.size());
}

void buffer::append(const void* d, size_type s)
{
    if (s != 0)
    {
        size_type ns(_size + s);

        if (_capacity < ns)
        {
            capacity(ns);
        }

        std::memcpy(_data + _size, d, s);
        _size = ns;
    }
}

void buffer::append(buffer&& x)
{
    if(_size == 0)
    {
        delete[] _data;
        _data = x._data;
        _size = x._size;
        _capacity = x._capacity;
    }
    else
    {
        append(x.data(), x.size());
    }
    x._data = nullptr;
    x._size = 0;
    x._capacity = 0;
}

void buffer::fill(value_type v)
{
    if (_size > 0)
    {
        std::memset(_data, v, _size);
    }
}

buffer::size_type buffer::size() const
{
    return _size;
}

void buffer::size(size_type s)
{
    if(_capacity < s)
    {
        capacity(s);
    }
    _size = s;
}

buffer::size_type buffer::capacity() const
{
    return _capacity;
}

void buffer::capacity(size_type c)
{
    value_type* d(new value_type[c]);

    if(_size > c)
    {
        _size = c;
    }

    if(_size != 0)
    {
        std::memcpy(d, _data, _size);
    }

    delete[] _data;
    _data = d;
    _capacity = c;
}

bool buffer::empty() const
{
    return _size == 0;
}

void buffer::clear()
{
    _size = 0;
}

buffer::value_type* buffer::data()
{
    return _data;
}

const buffer::value_type* buffer::data() const
{
    return _data;
}

buffer::value_type& buffer::operator[](size_type i)
{
    return _data[i];
}

const buffer::value_type& buffer::operator[](size_type i) const
{
    return _data[i];
}

buffer::value_type& buffer::at(size_type i)
{
    if (i >= _size)
    {
        throw std::out_of_range("index out of range");
    }

    return _data[i];
}

const buffer::value_type& buffer::at(size_type i) const
{
    if (i >= _size)
    {
        throw std::out_of_range("index out of range");
    }

    return _data[i];
}

buffer::size_type buffer::find(value_type v, size_type pos) const
{
    if(_size != 0 && pos < _size)
    {
        size_type n(pos);

        for(; n<_size; n++)
        {
            if(_data[n] == v)
            {
                return n;
            }
        }
    }
    return npos;
}

buffer::size_type buffer::rfind(value_type v, size_type pos) const
{
    if(_size != 0)
    {
        size_type n(_size);

        if(--n > pos)
        {
            n = pos;
        }

        for(++n; n-- != 0;)
        {
            if(_data[n] == v)
            {
                return n;
            }
        }
    }
    return npos;
}

bool operator==(const buffer& a, const buffer& b)
{
  return a.size() == b.size() &&
    std::memcmp(a.data(), b.data(), a.size()) == 0;
}

bool operator!=(const buffer& a, const buffer& b)
{
  return !(a == b);
}

