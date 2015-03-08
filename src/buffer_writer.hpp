#ifndef __buffer_writer__
#define __buffer_writer__

#include <cstdint>
#include <string>
#include <initializer_list>

class buffer;
class buffer_reader;

class buffer_writer
{
public:
    typedef std::size_t size_type;
    typedef uint8_t value_type;

    explicit buffer_writer(buffer& buffer, bool fit=false);

    size_type advance(size_type size);
	size_type fill(value_type value, size_type size=1);
	size_type write(const void* in, size_type size);
	size_type write(const buffer& in, size_type size=-1, size_type pos=0);
	size_type write(buffer_reader& in, size_type size);
    size_type write(const std::string& in);

    template<typename T>
    size_type write(std::initializer_list<T> list);

    bool end() const;
    size_type pos() const;

private:
    buffer& _buffer;
    size_type _pos;
    bool _fit;

    size_type fit(size_type size) const;
};

template<typename T>
buffer_writer::size_type buffer_writer::write(std::initializer_list<T> list)
{
    return write(list.begin(), sizeof(T)*list.size());
}

#endif
