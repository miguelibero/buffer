Buffer
======

A C++11 class to maintain binary data.

The code is based on this implementation:
http://www.codesynthesis.com/~boris/blog/2011/08/09/do-we-need-std-buffer/

A `buffer` object maintains a pointer, a size and a capacity.

```c++
buffer b(43);

b.size(); // 43
b.capacity(); // 43
b.data(); // pointer to the data
```

Use a `buffer_reader` to read from a buffer.

```c++
buffer b("first line\nsecond line");
buffer_reader reader(b);

std::string line;
reader.read(line); // will write "first line" into string
reader.read(line); // will write "second line" into string
```

Use a `buffer_writer` to write to the buffer.

```c++
buffer b;
buffer_writer writer(b);

writer.write({1, 2, 3});
```

Check the header files and the unit tests to se the available methods.
