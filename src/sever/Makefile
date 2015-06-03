CFLAGS += -pthread -I../include -g
LFLAGS += -pthread

all: server
#server: main.o thr_channel.o thr_list.o medialib.o mytbf.o
server: main.o thr_channel.o medialib.o mytbf.o 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	rm *.o server -rf
