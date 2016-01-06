#include <buffer.hpp>
#include <buffer_writer.hpp>
#include <buffer_reader.hpp>
#include <cstring>
#include "catch.hpp"

TEST_CASE("buffer_writer_constructor", "[buffer_writer]")
{
    buffer b;
    buffer_writer w(b);

	REQUIRE(w.pos() == 0);
}


TEST_CASE("write_pointer", "[buffer_writer]")
{
    buffer b;
    buffer_writer w(b);

    auto s = w.write("lala", 4);

	REQUIRE(s == 4);
	REQUIRE(b.size() == 4);
	REQUIRE(b.capacity() == 4);
	REQUIRE(w.pos() == 4);

    b.capacity(10);
    w.write("lolo", 5);

	REQUIRE(b.size() == 9);
	REQUIRE(b.capacity() == 10);
	REQUIRE(w.pos() == 9);
	REQUIRE(strcmp(reinterpret_cast<const char*>(b.data()), "lalalolo") == 0);
}

TEST_CASE("write_value", "[buffer_writer]")
{
    buffer b;
    buffer_writer w(b);

    w.fill('a', 4);
    w.fill(0);

	REQUIRE(b.size() == 5);
	REQUIRE(b.capacity() == 5);
	REQUIRE(w.pos() == 5);
	REQUIRE(strcmp(reinterpret_cast<const char*>(b.data()), "aaaa") == 0);

}

TEST_CASE("write_buffer", "[buffer_writer]")
{
    buffer out;
    buffer_writer writer(out);
    buffer in("hellohello", 11);

    writer.write(in, 5);
    writer.fill(0);

	REQUIRE(out.size() == 6);
	REQUIRE(out.capacity() == 6);
	REQUIRE(writer.pos() == 6);
	REQUIRE(strcmp(reinterpret_cast<const char*>(out.data()), "hello") == 0);

}

TEST_CASE("write_reader", "[buffer_writer]")
{
    buffer in("lala\nlolo\nli", 13);
    buffer out;
    buffer_reader reader(in);
    buffer_writer writer(out);

    writer.write(reader, 5);
    reader.advance(5);
    auto s = writer.write(reader, 5);
	REQUIRE(s == 3);
	REQUIRE(strcmp(reinterpret_cast<const char*>(out.data()), "lala\nli") == 0);
	REQUIRE(out.capacity() == 8);
	REQUIRE(out.size() == 8);

}

TEST_CASE("write_string", "[buffer_writer]")
{
    buffer out;
    buffer_writer writer(out);

    writer.write("lala");
    writer.write("lolo");
    writer.fill(0);

	REQUIRE(strcmp(reinterpret_cast<const char*>(out.data()), "lalalolo") == 0);
	REQUIRE(out.capacity() == 9);
	REQUIRE(out.size() == 9);
}

TEST_CASE("write_fit", "[buffer_writer]")
{
    buffer out(5);
    buffer_writer writer(out, true);

    auto s = writer.write("lalalololulu");
	REQUIRE(s == 5);
	REQUIRE(out.capacity() == 5);
	REQUIRE(out.size() == 5);
}
