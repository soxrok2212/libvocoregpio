CC=gcc
CFLAGS=-Wall
BINS=libvocoregpio.a libvocoregpio.so libvocoregpio.o

all: $(BINS)

libvocoregpio.o: libvocoregpio.c vocoregpio.h
	$(CC) $(CFLAGS) -c libvocoregpio.c

libvocoregpio.so: libvocoregpio.c vocoregpio.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libvocoregpio.c -lc	

libvocoregpio.a: libvocoregpio.o
	ar rcs libvocoregpio.a libvocoregpio.o
clean:
	rm *.o *.so *.a
