CC = gcc
SRC = ./example/main.c ./example/sokol.c
OUT = ./example/main
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