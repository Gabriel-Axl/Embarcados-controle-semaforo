
BINFOLDER := bin/

INCFOLDER := inc/

SRCFOLDER := src/

OBJFOLDER := obj/
CC := gcc
CFLAGS := -std=c99 -w
LINKFLAGS := -pthread 
SRCFILES := $(wildcard src/*.c)
CONFIG_FILE=`grep "^[^\#*/;]" $(FILE)`

porta1 := 11061
ip1:= "127.0.0.1"
ip2:= "127.0.0.1"
porta2 := 11062
all: $(SRCFILES:src/%.c=obj/%.o)
	$(CC) $(CFLAGS) obj/*.o -o bin/main $(LINKFLAGS)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I./inc $(LINKFLAGS)

run: bin/main
	bin/main $(porta1) $(ip1) $(porta2) $(ip2)

.PHONY: clean
clean:
	rm -rf obj/.o
	rm -rf bin/main
	touch obj/.gitkeep
	touch bin/.gitkeep