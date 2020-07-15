# libber

libber is a library for parsing data encoded with [Basic Encoding Rules](https://en.wikipedia.org/wiki/X.690#BER_encoding).

# Pre-requisites

To build only the C library, the following packages are required:

* gcc
* CMake
* pod2man (only required for manpages)

# Installation

```bash
mkdir build
cd build
cmake ...
make install
```

# Quickstart

A quick example of how to use libber:

```c
#include <ber.h>

#include <errno.h>
...
struct ber_element element;
struct ber_parser_new* parser = ber_parser_new();
FILE* fp = fopen("file.ber","rb");

ber_parser_set_input(parser, input);
while (ber_parser_parse(&element, parser) == 0 || errno == EAGAIN) {
  // Do stuff with element
}
ber_parser_free(parser);
fclose(fp);
```

# Documentation

Documentation can be found at https://reverentengineer.com/ber

# License

This software is released under the MIT license. See the file LICENSE for more details.
