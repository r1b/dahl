CC = clang
CFLAGS += -I include/ -I contrib/include/ -Weverything -Wno-gnu-case-range -Wno-padded -g
LDFLAGS += -shared
SRC = $(wildcard src/*.c)
TESTSRC = $(wildcard test/*.c)

debug:
	$(CC) $(CFLAGS) -o build/dahl $(SRC)
lib:
	$(CC) $(CFLAGS) $(LDFLAGS) -o build/libdahl.dylib $(SRC)
test:
	$(CC) $(CFLAGS) -Lbuild -lcheck -ldahl -o build/test-dahl $(TESTSRC)
