include ${SSFS_ROOT_DIR}/cc.mk

all: fd.o

fd.o: Fd.cpp Fd.h
	$(CC) $(CC_FLAGS) -c Fd.cpp -o $(BIN_DIR)/Fd.o

clean:
	rm *.o 2> /dev/null || true
