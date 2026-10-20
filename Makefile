CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -g -pthread
TARGET = billing_engine

SRCS = src/main.c src/database.c src/network.c src/billing.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)
