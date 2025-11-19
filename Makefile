# ============================================================
# Makefile for Coffee Shop Ordering System
# ------------------------------------------------------------
# This Makefile automates compilation of all project modules.
# It builds the final executable "coffee_shop".
# ============================================================

# Compiler to use
CC = clang

# Compiler flags:
#  -std=c99 : use C99 standard
#  -Wall    : enable most warnings for safer code
CFLAGS = -std=c99 -Wall

# Object files needed for the final program
OBJS = main.o manager.o customer.o data.o util.o

# Final output executable name
TARGET = coffee_shop

# ------------------------------------------------------------
# Build final executable from all object files
# ------------------------------------------------------------
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# ------------------------------------------------------------
# Compile source files into object files
# Each .o depends on its .c file and the shared header
# ------------------------------------------------------------

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

# ------------------------------------------------------------
# clean: remove all object files and the compiled executable
# ------------------------------------------------------------
clean:
	rm -f $(OBJS) $(TARGET)