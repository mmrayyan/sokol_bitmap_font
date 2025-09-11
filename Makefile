CC = gcc
SRC = ./test/main.c ./test/sokol.c ./sokol_bitmap.c
OUT = ./test/main
INCLUDE = ./include
CFLAGS = -mwin32 -I$(INCLUDE)
LDFLAGS = -lkernel32 -luser32 -lshell32 -lgdi32

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

run: all
	$(OUT)

clean:
	rm ./test/main.exe

.PHONY: all run clean