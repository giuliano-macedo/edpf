all:bbmed benchmark
LIBFLAGS := -std=gnu11
IFLAGS := -Iinclude/
BASESRC := src/tree.c src/ntree.c src/avl.c src/navl.c src/hashtable.c src/utils.c
BBMEDSRC := $(BASESRC) src/bbmed.c
BENCHSRC := $(BASESRC) src/benchmark.c

BASEHDR  := include/base.h include/tree.h include/ntree.h include/avl.h include/navl.h include/hashtable.h include/macros.h
BBMEDHDR := $(BASEHDR) include/bbmed.h
BENCHHDR := $(BASEHDR) include/benchmark.h
bbmed: $(BBMEDSRC) $(BBMEDHDR)
	gcc $(BBMEDSRC) $(IFLAGS) $(LIBFLAGS) -o bbmed
benchmark:$(BENCHSRC) $(BENCHHDR)
	gcc $(LIBFLAGS) $(IFLAGS) $(BENCHSRC) -o benchmark
.PHONY clean:
clean:
	rm -f benchmark bbmed