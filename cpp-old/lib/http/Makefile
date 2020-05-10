include ${SSFS_ROOT_DIR}/cc.mk

all: response.o request.o

response.o: response.cpp response.h
	$(CC) $(CC_FLAGS) -c response.cpp

request.o: request.cpp request.h
	$(CC) $(CC_FLAGS) -c request.cpp

clean:
	rm *.o 2> /dev/null || true
	$(MAKE) -C parse clean
