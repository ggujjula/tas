INCLUDE_FLAGS=-I/root/tas/lib/tas/include

LIBTAS=/root/tas/lib
LD_DIRS=-L$(LIBTAS)
LD_LIBS=-ltas

all: compile link

compile:
	gcc -c -Wall -Werror $(INCLUDE_FLAGS) -o main.o tas_test.c

link: main.o
	gcc -Wall -Werror $(LD_DIRS) -Wl,-rpath=$(LIBTAS) -o tas_test main.o $(LD_LIBS)

clean:
	rm tas_test main.o
