# Coffee Shop System Makefile

CC = gcc
CFLAGS = -std=c99 -Wall
OBJS = main.o manager.o customer.o data.o util.o
TARGET = coffee_shop

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c coffee_shop.h
	$(CC) $(CFLAGS) -c main.c

manager.o: manager.c coffee_shop.h
	$(CC) $(CFLAGS) -c manager.c

customer.o: customer.c coffee_shop.h
	$(CC) $(CFLAGS) -c customer.c

data.o: data.c coffee_shop.h
	$(CC) $(CFLAGS) -c data.c

util.o: util.c coffee_shop.h
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -f $(OBJS) $(TARGET)
