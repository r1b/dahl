all:
	clang -g -o dahl -Weverything -Wno-gnu-case-range -Wno-padded *.c
