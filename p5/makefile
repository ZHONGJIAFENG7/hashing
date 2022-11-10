CFLAGS = `pkg-config sdl2 --cflags` -O2 -Wall -Wextra -Wfloat-equal -pedantic -ansi -lm
INCS = custom_sdl2.h dhash.h shash.h exthash.h
TARGET = extend
SOURCES =  custom_sdl2.c dhash.c shash.c exthash.c $(TARGET).c
LIBS =  `pkg-config sdl2 --libs`
CC = gcc


all: $(TARGET)

$(TARGET): $(SOURCES) $(INCS)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

run: all
	$(TARGET)
