CC = gcc
TARGET = minesweeper
SOURCES = src/*.c
FLAGS = -o $(TARGET) -Iinclude -lncurses -lm

$(TARGET): $(SOURCES)
	$(CC) $(FLAGS) $(SOURCES)
