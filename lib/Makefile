CC = clang++
CC_VERSION = -std=c++1y
PROJECT_ROOT_DIR = /home/bb/code/cpp/ssfs
CWD = $(shell pwd)
CC_FLAGS = $(CC_VERSION) -Wall -Werror -I$(PROJECT_ROOT_DIR)

all: net fs run unit_test html http server

net:
	$(MAKE) -C net

fs:
	$(MAKE) -C fs

run:
	$(MAKE) -C run

unit_test:
	$(MAKE) -C unit_test

html:
	$(MAKE) -C html

http:
	$(MAKE) -C http

clean:
	$(MAKE) -C fs clean
	$(MAKE) -C html clean
	$(MAKE) -C http clean
	$(MAKE) -C net clean
	$(MAKE) -C run clean
	$(MAKE) -C string clean
	$(MAKE) -C unit_test clean
