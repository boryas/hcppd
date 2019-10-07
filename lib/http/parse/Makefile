include ${SSFS_ROOT_DIR}/cc.mk

all: parser.o
test: parser_test

parser_test: parser_test.o
	$(CC) $(CC_FLAGS) parser_test.o parser.o $(SSFS_ROOT_DIR)/lib/http/request.o $(SSFS_ROOT_DIR)/lib/unit_test/unit_test.o $(SSFS_ROOT_DIR)/lib/string/view.o -o parser_test

parser_test.o: test/parser_test.cpp parser.o $(SSFS_ROOT_DIR)/lib/http/request.o $(SSFS_ROOT_DIR)/lib/unit_test/unit_test.o $(SSFS_ROOT_DIR)/lib/string/view.o
	$(CC) $(CC_FLAGS) -c test/parser_test.cpp

parser.o: parser.cpp parser.h $(SSFS_ROOT_DIR)/lib/http/request.o $(SSFS_ROOT_DIR)/lib/string/view.o
	$(CC) $(CC_FLAGS) -c parser.cpp

$(SSFS_ROOT_DIR)/lib/http/request.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/http request.o

$(SSFS_ROOT_DIR)/lib/unit_test/unit_test.o:
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/unit_test unit_test.o

$(SSFS_ROOT_DIR)/lib/string/view.o: $(SSFS_ROOT_DIR)/lib/string/view.cpp $(SSFS_ROOT_DIR)/lib/string/view.h
	$(MAKE) -C $(SSFS_ROOT_DIR)/lib/string view.o

clean:
	rm parser_test *.o 2> /dev/null || true
