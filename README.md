# MD5 implementation in C

A simple MD5 implementation written in C.

## Build

    gcc md5.c main.c -lm -o md5

## Usage

    ./md5 <filename>

## Tests

The tests do consist of the RFC 1321 examples and can be executed like this:

    gcc tests.c md5.c -lm -o md5_tests && ./md5_tests

## License

GNU **GPLv3**.  
A copy is available at LICENSE.md
