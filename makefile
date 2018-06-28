all:bbmed benchmark
LIBFLAGS := -fshort-enums -std=gnu11 -Wall -Wno-switch
#-fshort-enums para forçar tamanho de enums para ser o mínimo possível
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
	gcc -D BENCH $(LIBFLAGS) $(IFLAGS) $(BENCHSRC) -o benchmark
.PHONY clean:
clean:
	rm -f benchmark bbmed