CFLAGS += -I../include -pthread
LFLAGS += -pthread

all: client
client: main.o
	gcc $(LDFLAGS) -o $@ $^

clean:
	rm *.o client -rf
