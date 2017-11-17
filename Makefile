CC=clang
CFLAGS = -Wall -O3 -g
LFLAGS = 
SOURCES = common.c llist.c vector.c hashtable.c graph.c set.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = libDANG.a
INSTALLPATH = /usr/local/bin/

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o

$(EXECUTABLE): $(OBJECTS) 
	ar -rc libDANG.a  $(OBJECTS) 
	ranlib libDANG.a
.c.o:
	$(CC) -c $(CFLAGS) $(LFLAGS) $< -o $@

install:
	rm -dr /usr/include/DANG
	mkdir /usr/include/DANG
	cp DANG/* /usr/include/DANG/
	cp libDANG.a /usr/lib/
clean: 
	rm -f $(EXECUTABLE) *.o *~ 
