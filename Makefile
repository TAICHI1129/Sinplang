CC = gcc
CFLAGS = -Wall -O2
SRCS = src/main.c src/lexer.c src/parser.c src/interpreter.c
OBJS = $(SRCS:.c=.o)
TARGET = simplang

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJS) $(TARGET)