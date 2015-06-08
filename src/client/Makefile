CFLAGS+=-I../include -Wall

all: client
client: main.o
	gcc $(LDFLAGS) -o $@ $^

clean:
	rm *.o client -rf
