#include <buffer.hpp>
#include <cstring>
#include "gtest/gtest.h"

TEST(buffer, constructor1) {

    buffer b(43);

    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(43));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(43));

}

TEST(buffer, constructor2) {

    buffer b(43, 100);

    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(43));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(100));

}

TEST(buffer, constructor3) {

    buffer b("test", 5);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "test");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(5));
}


TEST(buffer, constructor4) {

    buffer b("test", 5, 10);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "test");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));
}

TEST(buffer, copy_constructor) {

    buffer b("test", 5, 10);
    buffer c(b);

    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "test");
    ASSERT_EQ(c.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(c.capacity(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(c, b);
}

TEST(buffer, move_constructor) {

    buffer b("test", 5, 10);
    auto ptr = b.data();
    buffer c(std::move(b));

    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "test");
    ASSERT_EQ(c.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(c.capacity(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(0));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(0));
    ASSERT_EQ(ptr, c.data());
}

TEST(buffer, copy_assign) {

    buffer b("test", 5, 10);
    buffer c;
    c = b;

    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "test");
    ASSERT_EQ(c.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(c.capacity(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(c, b);
}

TEST(buffer, move_assign) {

    buffer b("test", 5, 10);
    auto ptr = b.data();
    buffer c;
    c = std::move(b);

    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "test");
    ASSERT_EQ(c.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(c.capacity(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(0));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(0));
    ASSERT_EQ(ptr, c.data());
}

TEST(buffer, swap) {

    buffer b("test", 5, 10);
    auto bptr = b.data();
    buffer c("test2", 6, 20);
    auto cptr = c.data();
    b.swap(c);

    ASSERT_STREQ(reinterpret_cast<const char*>(c.data()), "test");
    ASSERT_EQ(c.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(c.capacity(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(6));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(20));
    ASSERT_EQ(bptr, c.data());
    ASSERT_EQ(cptr, b.data());

}

TEST(buffer, detach) {

    buffer::value_type* ptr;
    {
        buffer b("test", 5, 10);
        ptr = b.detach();
    }

    ASSERT_STREQ(reinterpret_cast<const char*>(ptr), "test");

    delete[] ptr;
}

TEST(buffer, assign) {

    buffer b;
    auto ptr = "test";
    b.assign(ptr, 5);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "test");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(5));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(5));
    ASSERT_NE(ptr, reinterpret_cast<const char*>(b.data()));
}

TEST(buffer, append_with_capacity) {

    buffer b("test", 4, 20);
    b.append("test2", 6);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "testtest2");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(20));

}

TEST(buffer, append_without_capacity) {

    buffer b("test", 4);
    b.append("test2", 6);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "testtest2");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));

}

TEST(buffer, append_buffer_with_capacity) {

    buffer b("test", 4, 20);
    buffer c("test2", 6, 20);
    b.append(c);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "testtest2");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(20));

}

TEST(buffer, append_buffer_without_capacity) {

    buffer b("test", 4);
    buffer c("test2", 6, 20);
    b.append(c);

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "testtest2");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));

}

TEST(buffer, append_buffer_move) {

    buffer b;
    buffer c("test2", 6, 20);
    b.append(std::move(c));

    ASSERT_STREQ(reinterpret_cast<const char*>(b.data()), "test2");
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(6));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(20));

}

TEST(buffer, fill) {

    buffer b(3);
    b.fill(0);

    ASSERT_TRUE(std::memcmp(b.data(), "\0\0\0", b.size()) == 0);

}

TEST(buffer, size) {

    buffer b(3);
    bool success = b.size(10);

    ASSERT_TRUE(success);
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(10));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));

    success = b.size(2);

    ASSERT_FALSE(success);
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(2));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));
}

TEST(buffer, capacity) {

    buffer b(3);
    bool success = b.capacity(10);

    ASSERT_TRUE(success);
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(3));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));

    success = b.capacity(2);

    ASSERT_FALSE(success);
    ASSERT_EQ(b.size(), static_cast<buffer::size_type>(3));
    ASSERT_EQ(b.capacity(), static_cast<buffer::size_type>(10));
}

TEST(buffer, empty) {

    buffer b;

    ASSERT_TRUE(b.empty());

    b.size(1);

    ASSERT_FALSE(b.empty());

    b.clear();

    ASSERT_TRUE(b.empty());
}

TEST(buffer, at) {

    buffer b("test", 4);

    ASSERT_EQ('s', b[2]);
    ASSERT_EQ('t', b.at(3));
}

TEST(buffer, find) {

    buffer b("abcabc", 7);

    ASSERT_EQ(b.find('c'), static_cast<buffer::size_type>(2));
    ASSERT_EQ(b.find('z'), buffer::npos);
}

TEST(buffer, rfind) {

    buffer b("abcabc", 7);

    ASSERT_EQ(b.rfind('c'), static_cast<buffer::size_type>(5));
    ASSERT_EQ(b.rfind('z'), buffer::npos);
}

TEST(buffer, compare) {

    buffer b("abcabc", 7);
    buffer c("abcabc", 7);

    ASSERT_EQ(b, c);

    c.capacity(10);

    ASSERT_EQ(b, c);

    c.size(4);

    ASSERT_NE(b, c);

}


