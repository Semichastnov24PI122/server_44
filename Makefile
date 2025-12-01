CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -D_GNU_SOURCE
LIBS = -lssl -lcrypto

SOURCES = server.c logger.c database.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
