CC=gcc -std=c11
CFLAGS=-g
LDFLAGS=
SOURCES=main.c huffman.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=huffman

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE):	$(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)
