CC = gcc
CFLAGS = -lrt -lpthread

all: writer reader

writer: writer.c
	$(CC) writer.c -o writer $(CFLAGS)

reader: reader.c
	$(CC) reader.c -o reader $(CFLAGS)

clean:
	rm -f writer reader
