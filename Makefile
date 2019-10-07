include ${SSFS_ROOT_DIR}/cc.mk

all: blog_server

blog_server:
	$(MAKE) -C servers/blog
	cp servers/blog/blogd .

clean:
	rm *.o blogd 2> /dev/null || true
	$(MAKE) -C lib clean
	$(MAKE) -C servers/blog clean
