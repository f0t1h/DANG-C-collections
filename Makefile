CC=clang
CFLAGS = -Wall -O3 -g -IDANG
SOURCES = common.c llist.c vector.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = libDANG.a
INSTALLPATH = /usr/local/bin/

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o

$(EXECUTABLE): $(OBJECTS) 
	ar -rc libDANG.a  $(OBJECTS) 

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f $(EXECUTABLE) *.o *~ 
