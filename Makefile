CC=gcc
CFLAGS=-std=c90 -Wall -Wextra -Wpedantic
LIBS=
OBJ=main.o slcsp.o list.o plan.o zip.o

a.out: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	-rm *.o 2>/dev/null
	-rm main
.PHONY: clean
