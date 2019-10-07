include ${SSFS_ROOT_DIR}/cc.mk

all: fd.o fs.o

fd.o: fd.cpp fd.h
	$(CC) $(CC_FLAGS) -c fd.cpp

fs.o: fs.cpp fs.h
	$(CC) $(CC_FLAGS) -c fs.cpp

clean:
	rm *.o 2> /dev/null || true
