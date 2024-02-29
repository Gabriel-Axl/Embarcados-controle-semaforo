
BINFOLDER := bin/

INCFOLDER := inc/

SRCFOLDER := src/

OBJFOLDER := obj/
CC := gcc
CFLAGS := -std=c99 -w
LINKFLAGS := -lwiringPi -pthread 
SRCFILES := $(wildcard src/*.c)
CONFIG_FILE=`grep "^[^\#*/;]" $(FILE)`
cruzamento := 1
porta := 11061
ip := 127.0.0.1
all: $(SRCFILES:src/%.c=obj/%.o)
	$(CC) $(CFLAGS) obj/*.o -o bin/main $(LINKFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I./inc $(LINKFLAGS)

run: bin/main
	bin/main ${porta} ${ip} ${cruzamento}

.PHONY: clean
clean:
	rm -rf obj/*.o
	rm -rf bin/main
	touch obj/.gitkeep
	touch bin/.gitkeep