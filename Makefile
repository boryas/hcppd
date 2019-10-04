CC = clang++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: blog_server

blog_server:
	$(MAKE) -C servers/blog
	cp servers/blog/blogd .

clean:
	rm *.o blogd 2> /dev/null || true
	$(MAKE) -C lib clean
	$(MAKE) -C servers/blog clean
