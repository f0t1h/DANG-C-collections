CC=clang
CFLAGS = -Wall -O3 -g
LFLAGS = 
SOURCES = common.c llist.c vector.c
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

clean: 
	rm -f $(EXECUTABLE) *.o *~ 
