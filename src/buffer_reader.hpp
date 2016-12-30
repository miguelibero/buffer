#ifndef __buffer_reader__
#define __buffer_reader__

#include <cstdint>
#include <string>

class buffer;
class buffer_writer;

class buffer_reader
{
public:
    typedef std::size_t size_type;
    typedef uint8_t value_type;

    explicit buffer_reader(const buffer& buffer);

    size_type advance(size_type size);
    size_type read(void* out, size_type size);
    size_type read(buffer_writer& out, size_type size);
    size_type read(buffer& out, size_type size);
    size_type readline(std::string& out, char end='\n');
    size_type readline(const char*& out, char end='\n');

    bool end() const;
    size_type pos() const;
    size_type size() const;

private:
    const buffer& _buffer;
    size_type _pos;

    size_type fit(size_type size) const;
};


#endif
