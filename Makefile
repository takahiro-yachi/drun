CFLAGS+= -g
CFLAGS+= -Wall
all:
	$(CC) -o drun $(CFLAGS) *.c
clean:
	$(RM) drun
