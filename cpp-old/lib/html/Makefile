include ${SSFS_ROOT_DIR}/cc.mk

all: template.o

template.o: template.cpp template.h $(SSFS_ROOT_DIR)/lib/fs/fs.o
	$(CC) $(CC_FLAGS) -c template.cpp

$(SSFS_ROOT_DIR)/lib/fs/fs.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/fs fs.o

uri_map.o: uri_map.cpp uri_map.h template.o
	$(CC) $(CC_FLAGS) -c uri_map.cpp

clean:
	rm *.o 2> /dev/null || true
