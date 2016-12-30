
#include "buffer_reader.hpp"
#include "buffer_writer.hpp"
#include "buffer.hpp"
#include <cstring>

buffer_reader::buffer_reader(const buffer& buffer):
_buffer(buffer), _pos(0)
{
}

buffer_reader::size_type buffer_reader::fit(size_type size) const
{
    if(_pos + size > _buffer.size())
    {
        size = _buffer.size() - _pos;
    }
    return size;
}

buffer_reader::size_type buffer_reader::advance(size_type size)
{
    size = fit(size);
    _pos += size;
    return size;
}

buffer_reader::size_type buffer_reader::read(void* out, size_type size)
{
    size = fit(size);
    if(size > 0)
    {
        std::memcpy(out, &_buffer.at(_pos), size);
        _pos += size;
    }
    return size;
}

buffer_reader::size_type buffer_reader::read(buffer_writer& out, size_type size)
{
    size = fit(size);
    if(size > 0)
    {
        out.write(&_buffer.at(_pos), size);
        _pos += size;
    }
    return size;
}

buffer_reader::size_type buffer_reader::read(buffer& out, size_type size)
{
    size = fit(size);
    if(size > 0)
    {
        out.assign(&_buffer.at(_pos), size);
        _pos += size;
    }
    return size;
}

buffer_reader::size_type buffer_reader::readline(std::string& s, char endchr)
{
    const char* ptr;
    auto size = readline(ptr, endchr);
    if(ptr == nullptr)
    {
        s.clear();
    }
    else
    {
        s = std::string(ptr, size);
    }
    return size;
}

buffer_reader::size_type buffer_reader::readline(const char*& out, char endchr)
{
    size_type endpos = _buffer.find(endchr, _pos);
    if(endpos == buffer::npos)
    {
        endpos = _buffer.empty() ? 0 : _buffer.size() - 1;
    }
    size_type size = endpos - _pos;
    if(size > 0)
    {
        out = reinterpret_cast<const char*>(&_buffer.at(_pos));
        _pos += size;
    }
    else
    {
        size = 0;
        out = nullptr;
    }
    if(!end())
    {
        _pos++;
    }
    return size;
}

bool buffer_reader::end() const
{
    return _pos >= size();
}

buffer_reader::size_type buffer_reader::pos() const
{
    return _pos;
}

buffer_reader::size_type buffer_reader::size() const
{
    return _buffer.size();
}
