Simple Software From Scratch
=====

A pile of libraries for writing servers. Everything is implemented using only C++ standard libraries and libc.

My main goal is to learn more about systems programming and C++. The project that motivates the libraries is an http server that hosts a blog. A side goal is for the blog server to host a blog about what I learn while writing the libraries.

If anyone cares, I'd like ssfs to be pronounced like Sisyphus.

to build:
make

to run:
./restart.sh

supported options in blogd:
--port: set the port the server will listen on
