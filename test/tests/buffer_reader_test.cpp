#include <buffer.hpp>
#include <buffer_reader.hpp>
#include <cstring>
#include "gtest/gtest.h"

TEST(buffer_reader, constructor) {

    buffer b;
    buffer_reader r(b);

    ASSERT_EQ(r.pos(), static_cast<buffer_reader::size_type>(0));
}

TEST(buffer_reader, read_pointer) {

    buffer b("lalalolo", 9);
    buffer_reader r(b);
    auto c = new char[5];    
    c[4] = 0;
    auto s = r.read(c, 4);

    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(4));
    ASSERT_EQ(r.pos(), static_cast<buffer_reader::size_type>(4));
    ASSERT_STREQ(c, "lala");

    s = r.read(c, 10);

    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(5));
    ASSERT_STREQ(c, "lolo");
    
}

TEST(buffer_reader, read_buffer) {

    buffer b("lalalololi", 11);
    buffer_reader r(b);
    buffer c(5);
    c.fill(0);

    auto s = r.read(c, 4);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(4));
    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "lala");

    s = r.read(c, 4);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(4));
    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "lolo");

    s = r.read(c, 4);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(3));
    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "li");
}

