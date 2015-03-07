
#include "buffer_reader.hpp"
#include "buffer.hpp"
#include <cstring>

buffer_reader::buffer_reader(const buffer& buffer):
_buffer(buffer), _pos(0)
{
}

buffer_reader::size_type buffer_reader::adjust_size(size_type size) const
{
    if(_pos + size > _buffer.size())
    {
        size = _buffer.size() - _pos;
    }
    return size;
}

buffer_reader::size_type buffer_reader::advance(size_type size)
{
    size = adjust_size(size);
    _pos += size;
    return size;
}

buffer_reader::size_type buffer_reader::read(void* data, size_type size)
{
    size = adjust_size(size);
    std::memcpy(data, &_buffer.at(_pos), size);
    _pos += size;
    return size;
}

buffer_reader::size_type buffer_reader::read(buffer_writer& writer, size_type size)
{
    size = adjust_size(size);
    // TODO
    _pos += size;
    return size;
}

buffer_reader::size_type buffer_reader::read(buffer& out, size_type size)
{
    size = adjust_size(size);
    out.assign(&_buffer.at(_pos), size);
    _pos += size;
    return size;
}

buffer_reader::size_type buffer_reader::read(std::string& s, char end)
{
    size_type endpos = _buffer.find(end, _pos);
    if(endpos == buffer::npos)
    {
        endpos = _buffer.empty() ? 0 : _buffer.size() - 1;
    }
    size_type size = endpos - _pos;
    auto ptr = reinterpret_cast<const char*>(&_buffer.at(_pos));
    s = std::string(ptr, size);
    return size;
}

bool buffer_reader::end() const
{
    return _pos >= _buffer.size();
}

buffer_reader::size_type buffer_reader::pos() const
{
    return _pos;
}

