#ifndef __buffer__
#define __buffer__

#include <cstdint>

class buffer
{
public:
    typedef std::size_t size_type;
    typedef uint8_t value_type;
    static const size_type npos;

    ~buffer();

    explicit buffer(size_type size = 0);
    buffer(size_type size, size_type capacity);
    buffer(const void* data, size_type size);
    buffer(const void* data, size_type size, size_type capacity);

    buffer(const buffer& other);
    buffer(buffer&& other);
    buffer& operator=(const buffer& other);
    buffer& operator=(buffer&& other);

    void swap(buffer& other);
    value_type* detach();

    void assign(const void* data, size_type size);
    void append(const buffer& other);
    void append(const void* data, size_type size);
    void append(buffer&& other);
    void fill(value_type value = 0);

    size_type size() const;
    bool size(size_type);
    size_type capacity() const;
    bool capacity(size_type);
    bool empty() const;
    void clear();

    value_type* data();
    const value_type* data() const;

    value_type& operator[](size_type);
    const value_type& operator[](size_type) const;
    value_type& at(size_type);
    const value_type& at(size_type) const;

    size_type find(value_type value, size_type pos = 0) const;
    size_type rfind(value_type value, size_type pos = npos) const;

private:
    value_type* _data;
    size_type _size;
    size_type _capacity;
};

bool operator==(const buffer& a, const buffer& b);
bool operator!=(const buffer& a, const buffer& b);

#endif
