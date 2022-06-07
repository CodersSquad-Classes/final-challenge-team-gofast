CC = gcc 
CFLAGS  = -g -Wall
TARGET = pacman

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lncurses -lm

clean:
	$(RM) $(TARGET)

	