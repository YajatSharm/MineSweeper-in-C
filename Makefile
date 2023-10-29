CC = gcc
CFLAGS = -Wall -Wextra -std=c99
EXECUTABLE = main
SOURCE = main.c

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCE)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCE)

clean:
	rm -f $(EXECUTABLE)
