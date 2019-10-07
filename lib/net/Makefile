include ${SSFS_ROOT_DIR}/cc.mk

all: socket.o

socket.o: socket.cpp socket.h ${SSFS_ROOT_DIR}/lib/fs/fd.o
	$(CC) $(CC_FLAGS) -c socket.cpp

${SSFS_ROOT_DIR}/lib/fs/fd.o:
	$(MAKE) -C ${SSFS_ROOT_DIR}/lib/fs fd.o

clean:
	rm *.o 2> /dev/null || true
