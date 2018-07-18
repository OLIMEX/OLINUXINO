= C++ Library for GPIO

This small library provides some classes to access to GPIO of A20 boards from your C++ application.

This work is based on ``gpio_lib.c``, part of pyA20.

Before using this module, please review the [documentation](documentation.pdf).
You can find a simple working example under the [examples](examples) directory.

= Howto use the library

You can use the Makefile provided to compile the library, you can compile the two file directly in your project, or
you can link the static library provided (compiled on Linux a20-olimex 3.4.103+ / gcc 4.6.3).

To compile the example from the [examples](examples) directory:
`g++ ex.cpp -O2 -o ex.o -L.. -lA20-gpio`

= License

See the [LICENSE](LICENSE) file.

= Author

Federico Reghenzani - [email](federico.dev@reghe.net) - [GitHub](https://github.com/federeghe)
