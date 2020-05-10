include ${SSFS_ROOT_DIR}/cc.mk

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
