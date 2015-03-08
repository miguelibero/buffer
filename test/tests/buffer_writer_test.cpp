#include <buffer.hpp>
#include <buffer_writer.hpp>
#include <buffer_reader.hpp>
#include <cstring>
#include "gtest/gtest.h"

TEST(buffer_writer, constructor) {

    buffer b;
    buffer_writer w(b);

    ASSERT_EQ(w.pos(), static_cast<buffer_writer::size_type>(0));
}


TEST(buffer_writer, write_pointer) {

    buffer b;
    buffer_writer w(b);

    auto s = w.write("lala", 4);

    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(4));
    ASSERT_EQ(b.size(), static_cast<buffer_writer::size_type>(4));
    ASSERT_EQ(b.capacity(), static_cast<buffer_writer::size_type>(4));
    ASSERT_EQ(w.pos(), static_cast<buffer_writer::size_type>(4));

    b.capacity(10);
    w.write("lolo", 5);

    ASSERT_EQ(b.size(), static_cast<buffer_writer::size_type>(9));
    ASSERT_EQ(b.capacity(), static_cast<buffer_writer::size_type>(10));
    ASSERT_EQ(w.pos(), static_cast<buffer_writer::size_type>(9));
    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "lalalolo");
}

TEST(buffer_writer, write_value) {

    buffer b;
    buffer_writer w(b);

    w.write('a', 4);
    w.write(0);

    ASSERT_EQ(b.size(), static_cast<buffer_writer::size_type>(5));
    ASSERT_EQ(b.capacity(), static_cast<buffer_writer::size_type>(5));
    ASSERT_EQ(w.pos(), static_cast<buffer_writer::size_type>(5));
    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "aaaa");
    
}

TEST(buffer_writer, write_buffer) {

    buffer out;
    buffer_writer writer(out);
    buffer in("hellohello", 11); 

    writer.write(in, 5);
    writer.write(0);

    ASSERT_EQ(out.size(), static_cast<buffer_writer::size_type>(6));
    ASSERT_EQ(out.capacity(), static_cast<buffer_writer::size_type>(6));
    ASSERT_EQ(writer.pos(), static_cast<buffer_writer::size_type>(6));
    ASSERT_STREQ(reinterpret_cast<const char*>(out.data()), "hello");
    
}

TEST(buffer_writer, write_reader) {

    buffer in("lala\nlolo\nli", 13);
    buffer out;
    buffer_reader reader(in);
    buffer_writer writer(out);

    writer.write(reader, 5);
    reader.advance(5);
    auto s = writer.write(reader, 5);
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(3));
    ASSERT_STREQ(reinterpret_cast<const char*>(out.data()), "lala\nli");
    ASSERT_EQ(out.capacity(), static_cast<buffer_reader::size_type>(8));
    ASSERT_EQ(out.size(), static_cast<buffer_reader::size_type>(8));

}

TEST(buffer_writer, write_string) {

    buffer out;
    buffer_writer writer(out);

    writer.write("lala");
    writer.write("lolo");
    writer.write(0);

    ASSERT_STREQ(reinterpret_cast<const char*>(out.data()), "lalalolo");
    ASSERT_EQ(out.capacity(), static_cast<buffer_reader::size_type>(9));
    ASSERT_EQ(out.size(), static_cast<buffer_reader::size_type>(9));
}

TEST(buffer_writer, write_fit) {

    buffer out(5);
    buffer_writer writer(out, true);

    auto s = writer.write("lalalololulu");
    ASSERT_EQ(s, static_cast<buffer_reader::size_type>(5));
    ASSERT_EQ(out.capacity(), static_cast<buffer_reader::size_type>(5));
    ASSERT_EQ(out.size(), static_cast<buffer_reader::size_type>(5));
}

