DANG_VERSION := "0.01-prerelease"
BUILD_DATE := "$(shell date)"
ODIR= objects
CC=clang
DEPS= common.h
CFLAGS= -O3 -DDANG_VERSION=\"$(DANG_VERSION)\" -DDANG_BUILD_DATE=\"$(BUILD_DATE)\"
SOURCES = dang.c llist.c vector.c common.c
_OBJ = $(SOURCES:.c=.o)
OBJECTS = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: DANG

DANG: $(OBJECTS)
	ar -rcs libDANG.a $(ODIR)/dang.o
$(ODIR)/%.o: %.c $(DEPS) 
	$(CC) -c $(CFLAGS) $< -o $@

common:
	$(CC) -c common.c -o common.o
clean:
	rm $(ODIR)/*.o -f
	rm libDang.a
