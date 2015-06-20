C++ Library for GPIO
--------------------
This small library provides some classes to access to GPIO of A20 boards from your C++ application.

This work is based on gpio_lib.c, part of pyA20.

Before use this module, please check the documentation (`documentation.pdf`). You can find a simple working example under
the `examples` directory.

Howto use the library
---------------------
You can use the Makefile provided to compile the library, you can compile the two file directly in your project, or
you can link the static library provided (compiled on Linux a20-olimex 3.4.103+ / gcc 4.6.3).

To compile the example from the `examples` directory:
`g++ ex.cpp -O2 -o ex.o -L.. -lA20-gpio`

License
-------
See LICENSE file.

Author
------
Federico Reghenzani &lt;federico.dev@reghe.net&gt; - https://github.com/federeghe
