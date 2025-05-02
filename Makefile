CC=gcc
CFLAGS=-Wall -pthread
TARGET=validator

all: $(TARGET)

$(TARGET): validator.c
	$(CC) $(CFLAGS) -o $(TARGET) validator.c

clean:
	rm -f $(TARGET)

.PHONY: all clean