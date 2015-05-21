CFLAGS+=-I../include

all: client
client: main.o
	gcc $(LDFLAGS) -o $@ $^

clean:
	rm *.o client -rf
