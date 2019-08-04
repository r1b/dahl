debug:
	clang -g -o build/dahl -Weverything -Wno-gnu-case-range -Wno-padded src/*.c
lib:
	clang -o build/libdahl.dylib -shared src/*.c
test:
	clang -Lbuild -lcheck -ldahl -o build/test-dahl tests/*.c && ./build/test-dahl
