#include <buffer.hpp>
#include <buffer_reader.hpp>
#include <buffer_writer.hpp>
#include <cstring>
#include "catch.hpp"

TEST_CASE("buffer_reader_constructor", "[buffer_reader]")
{
	buffer b;
	buffer_reader r(b);

	REQUIRE(r.pos() == 0);
}

TEST_CASE("read_pointer", "[buffer_reader]")
{
	buffer b("lalalolo", 9);
	buffer_reader r(b);
	auto c = new char[5];
	c[4] = 0;
	auto s = r.read(c, 4);

	REQUIRE(s == 4);
	REQUIRE(r.pos() == 4);
	REQUIRE(strcmp(c, "lala") == 0);

	s = r.read(c, 10);

	REQUIRE(s == 5);
	REQUIRE(strcmp(c, "lolo") == 0);
}

TEST_CASE("read_buffer", "[buffer_reader]")
{
	buffer b("lalalololi", 11);
	buffer_reader r(b);
	buffer c(5);
	c.fill(0);

	auto s = r.read(c, 4);
	REQUIRE(s == 4);
	REQUIRE(strcmp(c.c_str(), "lala") == 0);

	s = r.read(c, 4);
	REQUIRE(s == 4);
	REQUIRE(strcmp(c.c_str(), "lolo") == 0);

	s = r.read(c, 4);
	REQUIRE(s == 3);
	REQUIRE(strcmp(c.c_str(), "li") == 0);
}

TEST_CASE("read_string", "[buffer_reader]")
{
	buffer b("lala\nlolo\nli\n\n", 14);
	buffer_reader r(b);
	std::string str;

	auto s = r.read(str);
	REQUIRE(s == 5);
	REQUIRE(str == "lala");

	s = r.read(str);
	REQUIRE(s == 5);
	REQUIRE(str == "lolo");

	s = r.read(str);
	REQUIRE(s == 3);
	REQUIRE(str == "li");

	for (int i = 0; i < 10; ++i)
	{
		s = r.read(str);
		REQUIRE(s == 0);
		REQUIRE(str == "");
	}
}

TEST_CASE("read_writer", "[buffer_reader]")
{
	buffer in("lala\nlolo\nli", 13);
	buffer out;
	buffer_reader reader(in);
	buffer_writer writer(out);

	reader.read(writer, 5);
	reader.advance(5);
	auto s = reader.read(writer, 5);
	REQUIRE(s == 3);
	REQUIRE(strcmp(out.c_str(), "lala\nli") == 0);
	REQUIRE(out.capacity() == 8);
	REQUIRE(out.size() == 8);
}
