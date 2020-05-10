include ${SSFS_ROOT_DIR}/cc.mk

all: view.o
test: view_test

view_test: view_test.o
	$(CC) $(CC_FLAGS) view_test.o view.o $(SSFS_ROOT_DIR)/lib/unit_test/unit_test.o -o view_test

view_test.o: test/view_test.cpp view.o $(SSFS_ROOT_DIR)/lib/unit_test/unit_test.o
	$(CC) $(CC_FLAGS) -c test/view_test.cpp

view.o: view.cpp view.h
	$(CC) $(CC_FLAGS) -c view.cpp

$(SSFS_ROOT_DIR)/lib/unit_test.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/unit_test unit_test.o

clean:
	rm *.o 2> /dev/null || true
