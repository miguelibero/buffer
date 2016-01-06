#include <buffer.hpp>
#include <cstring>
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("constructor1", "[buffer]")
{
    buffer b(43);

	REQUIRE(b.size() == 43);
	REQUIRE(b.capacity() == 43);
}

TEST_CASE("constructor2", "[buffer]")
{
    buffer b(43, 100);

	REQUIRE(b.size() == 43);
	REQUIRE(b.capacity() == 100);
}

TEST_CASE("constructor3", "[buffer]")
{
    buffer b("test", 5);

	REQUIRE(strcmp(b.c_str(), "test") == 0);
	REQUIRE(b.size() == 5);
	REQUIRE(b.capacity() == 5);
}

TEST_CASE("constructor4", "[buffer]")
{
    buffer b("test", 5, 10);

	REQUIRE(strcmp(b.c_str(), "test") == 0);
	REQUIRE(b.size() == 5);
	REQUIRE(b.capacity() == 10);
}

TEST_CASE("initializer_list", "[buffer]")
{
    buffer b({'t', 'e', 's', 't', '\0'});
	REQUIRE(strcmp(b.c_str(), "test") == 0);
	REQUIRE(b.size() == 5);
	REQUIRE(b.capacity() == 5);
}

TEST_CASE("copy_constructor", "[buffer]")
{
    buffer b("test", 5, 10);
    buffer c(b);

	REQUIRE(strcmp(c.c_str(), "test") == 0);
	REQUIRE(c.size() == 5);
	REQUIRE(c.capacity() == 10);
	REQUIRE(c == b);
}

TEST_CASE("move_constructor", "[buffer]")
{
    buffer b("test", 5, 10);
    auto ptr = b.data();
    buffer c(std::move(b));

	REQUIRE(strcmp(c.c_str(), "test") == 0);
	REQUIRE(c.size() == 5);
	REQUIRE(c.capacity() == 10);
	REQUIRE(b.size() == 0);
	REQUIRE(b.capacity() == 0);
	REQUIRE(ptr == c.data());
}

TEST_CASE("copy_assign", "[buffer]")
{
    buffer b("test", 5, 10);
    buffer c;
    c = b;

	REQUIRE(strcmp(c.c_str(), "test") == 0);
	REQUIRE(c.size() == 5);
	REQUIRE(c.capacity() == 10);
	REQUIRE(c == b);
}

TEST_CASE("move_assign", "[buffer]")
{
    buffer b("test", 5, 10);
    auto ptr = b.data();
    buffer c;
    c = std::move(b);

	REQUIRE(c.str() == "test");
	REQUIRE(c.size() == 5);
	REQUIRE(c.capacity() == 10);
	REQUIRE(b.size() == 0);
	REQUIRE(b.capacity() == 0);
	REQUIRE(ptr == c.data());
}

TEST_CASE("swap", "[buffer]")
{
    buffer b("test", 5, 10);
    auto bptr = b.data();
    buffer c("test2", 6, 20);
    auto cptr = c.data();
    b.swap(c);

	REQUIRE(strcmp(c.c_str(), "test") == 0);
	REQUIRE(c.size() == 5);
	REQUIRE(c.capacity() == 10);
	REQUIRE(b.size() == 6);
	REQUIRE(b.capacity() == 20);
	REQUIRE(bptr == c.data());
	REQUIRE(cptr == b.data());

}

TEST_CASE("detach", "[buffer]")
{
    buffer::value_type* ptr;
    {
        buffer b("test", 5, 10);
        ptr = b.detach();
    }

	REQUIRE(strcmp(reinterpret_cast<const char*>(ptr), "test") == 0);

    delete[] ptr;
}

TEST_CASE("assign", "[buffer]")
{
    buffer b;
    auto ptr = "test";
    b.assign(ptr, 5);

	REQUIRE(strcmp(b.c_str(), "test") == 0);
	REQUIRE(b.size() == 5);
	REQUIRE(b.capacity() == 5);
	REQUIRE(ptr != b.c_str());
}

TEST_CASE("append_with_capacity", "[buffer]")
{
    buffer b("test", 4, 20);
    b.append("test2", 6);

	REQUIRE(strcmp(b.c_str(), "testtest2") == 0);
	REQUIRE(b.size() == 10);
	REQUIRE(b.capacity() == 20);
}

TEST_CASE("append_without_capacity", "[buffer]")
{
    buffer b("test", 4);
    b.append("test2", 6);

	REQUIRE(strcmp(b.c_str(), "testtest2") == 0);
	REQUIRE(b.size() == 10);
	REQUIRE(b.capacity() == 10);

}

TEST_CASE("append_buffer_with_capacity", "[buffer]")
{
    buffer b("test", 4, 20);
    buffer c("test2", 6, 20);
    b.append(c);

	REQUIRE(strcmp(b.c_str(), "testtest2") == 0);
	REQUIRE(b.size() == 10);
	REQUIRE(b.capacity() == 20);

}

TEST_CASE("append_buffer_without_capacity", "[buffer]")
{
    buffer b("test", 4);
    buffer c("test2", 6, 20);
    b.append(c);

	REQUIRE(strcmp(b.c_str(), "testtest2") == 0);
	REQUIRE(b.size() == 10);
	REQUIRE(b.capacity() == 10);

}

TEST_CASE("append_buffer_move", "[buffer]")
{
    buffer b;
    buffer c("test2", 6, 20);
    b.append(std::move(c));

	REQUIRE(strcmp(b.c_str(), "test2") == 0);
	REQUIRE(b.size() == 6);
	REQUIRE(b.capacity() == 20);

}

TEST_CASE("fill", "[buffer]")
{
    buffer b(3);
    b.fill(0);

	REQUIRE(std::memcmp(b.data(), "\0\0\0", b.size()) == 0);
}

TEST_CASE("size", "[buffer]")
{
    buffer b(3);
    b.size(10);

	REQUIRE(b.size() == 10);
	REQUIRE(b.capacity() == 10);

    b.size(2);

	REQUIRE(b.size() == 2);
	REQUIRE(b.capacity() == 10);
}

TEST_CASE("capacity", "[buffer]")
{
    buffer b(3);
    b.capacity(10);

	REQUIRE(b.size() == 3);
	REQUIRE(b.capacity() == 10);

    b.capacity(2);

	REQUIRE(b.size() == 2);
	REQUIRE(b.capacity() == 2);
}

TEST_CASE("empty" "[buffer]")
{
    buffer b;

	REQUIRE(b.empty());

    b.size(1);

	REQUIRE(!b.empty());

    b.clear();

	REQUIRE(b.empty());
}

TEST_CASE("at" "[buffer]")
{
    buffer b("test", 4);

	REQUIRE('s' == b[2]);
	REQUIRE('t' == b.at(3));
}

TEST_CASE("find", "[buffer]")
{
    buffer b("abcabc", 7);

	REQUIRE(b.find('c') == 2);
	REQUIRE(b.find('z') == buffer::npos);
}

TEST_CASE("rfind", "[buffer]")
{
    buffer b("abcabc", 7);

	REQUIRE(b.rfind('c') == 5);
	REQUIRE(b.rfind('z') == buffer::npos);
}

TEST_CASE("compare", "[buffer]")
{
    buffer b("abcabc", 7);
    buffer c("abcabc", 7);

	REQUIRE(b == c);

    c.capacity(10);

	REQUIRE(b == c);

    c.size(4);

	REQUIRE(b != c);
}
