include ${SSFS_ROOT_DIR}/cc.mk

all: daemon.o options.o

daemon.o: daemon.cpp daemon.h
	$(CC) $(CC_FLAGS) -c daemon.cpp

options.o: options.cpp options.h
	$(CC) $(CC_FLAGS) -c options.cpp

clean:
	rm *.o 2> /dev/null || true
