all:
	clang -g -o build/dahl -Weverything -Wno-gnu-case-range -Wno-padded src/*.c
