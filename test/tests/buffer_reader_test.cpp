#include <buffer.hpp>
#include <buffer_reader.hpp>
#include <buffer_writer.hpp>
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

TEST(buffer_reader, read_string) {

    buffer b("lala\nlolo\nli\n\n", 14);
    buffer_reader r(b);
    std::string str;

    auto s = r.read(str);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(5));
    ASSERT_STREQ(str.c_str(), "lala");

    s = r.read(str);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(5));
    ASSERT_STREQ(str.c_str(), "lolo");

    s = r.read(str);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(3));
    ASSERT_STREQ(str.c_str(), "li");

    s = r.read(str);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(0));
    s = r.read(str);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(0));
}

TEST(buffer_reader, read_writer) {

    buffer in("lala\nlolo\nli", 13);
    buffer out;
    buffer_reader reader(in);
    buffer_writer writer(out);

    reader.read(writer, 5);
    reader.advance(5);
    auto s = reader.read(writer, 5);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(3));
    ASSERT_STREQ(reinterpret_cast<const char*>(out.data()), "lala\nli");
    ASSERT_EQ(out.capacity(), static_cast<buffer_reader::size_type>(8));
    ASSERT_EQ(out.size(), static_cast<buffer_reader::size_type>(8));
}

