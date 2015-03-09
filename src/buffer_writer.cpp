#include "buffer_writer.hpp"
#include "buffer.hpp"
#include "buffer_reader.hpp"
#include <cstring>

buffer_writer::buffer_writer(buffer& buffer, bool fit):
_buffer(buffer), _pos(0), _fit(fit)
{
}

buffer_writer::size_type buffer_writer::fit(size_type size) const
{
    if(_pos + size > _buffer.size())
    {
        if(!_fit)
        {
            _buffer.size(_pos + size);
        }
        else
        {
            size = _buffer.capacity() - _pos;
        }
    }
    return size;
}

buffer_writer::size_type buffer_writer::advance(size_type size)
{
    size = fit(size);
    _pos += size;
    return size;
}

buffer_writer::size_type buffer_writer::fill(value_type value, size_type size)
{
    size = fit(size);
    if(size > 0)
    {
        std::memset(&_buffer.at(_pos), value, size);
        _pos += size;
    }
    return size;
}

buffer_writer::size_type buffer_writer::write(const void* in, size_type size)
{
    size = fit(size);
    if(size > 0)
    {
        std::memcpy(&_buffer.at(_pos), in, size);
        _pos += size;
    }
    return size;
}

buffer_writer::size_type buffer_writer::write(const buffer& in, size_type size, size_type pos)
{
    if(size == static_cast<size_type>(-1))
    {
        size = in.size();
    }
    if(pos >= in.size())
    {
        pos = in.size();
    }
    if(pos + size > in.size())
    {
        size = in.size() - pos;
    }
    if(size > 0)
    {
        return write(&in.at(pos), size);
    }
    return size;
}

buffer_writer::size_type buffer_writer::write(buffer_reader& in, size_type size)
{
    if(in.pos() + size > in.size())
    {
        size = in.size() - in.pos();
    }
    size = fit(size);
    if(size > 0)
    {
        size = in.read(&_buffer.at(_pos), size);
        _pos += size;
    }
    return size;
}

buffer_writer::size_type buffer_writer::write(const std::string& s)
{
    size_type size = fit(s.size());
    if(size > 0)
    {
        return write(s.data(), size);
    }
    return size;
}

bool buffer_writer::end() const
{
    return _pos >= _buffer.capacity();
}

buffer_writer::size_type buffer_writer::pos() const
{
    return _pos;
}

