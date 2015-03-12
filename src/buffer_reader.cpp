
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

buffer_reader::size_type buffer_reader::read(std::string& s, char endchr)
{
    size_type endpos = _buffer.find(endchr, _pos);
    if(endpos == buffer::npos)
    {
        endpos = _buffer.empty() ? 0 : _buffer.size() - 1;
    }
    if(_pos >= endpos)
    {
        s = std::string();
        return 0;
    }
    size_type size = endpos - _pos;
    if(size > 0)
    {
        auto ptr = reinterpret_cast<const char*>(&_buffer.at(_pos));
        s = std::string(ptr, size);
        _pos += size;
        if(!end())
        {
            size++;
            _pos++;
        }
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

