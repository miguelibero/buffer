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
    std::memset(&_buffer.at(_pos), value, size);
    _pos += size;
    return size;
}

buffer_writer::size_type buffer_writer::write(const void* in, size_type size)
{
    size = fit(size);
    std::memcpy(&_buffer.at(_pos), in, size);
    _pos += size;
    return size;
}

buffer_writer::size_type buffer_writer::write(const buffer& in, size_type size, size_type pos)
{
    if(size == -1)
    {
        size = in.size();
    }
    if(pos + size > in.size())
    {
        size = in.size() - pos;
    }
    return write(&in.at(pos), size);
}

buffer_writer::size_type buffer_writer::write(buffer_reader& in, size_type size)
{
    if(in.pos() + size > in.size())
    {
        size = in.size() - in.pos();
    }
    size = fit(size);
    size = in.read(&_buffer.at(_pos), size);
    _pos += size;
    return size;
}

buffer_writer::size_type buffer_writer::write(const std::string& s)
{
    size_type size = fit(s.size());
    return write(s.data(), size);
}

bool buffer_writer::end() const
{
    return _pos >= _buffer.capacity();
}

buffer_writer::size_type buffer_writer::pos() const
{
    return _pos;
}

