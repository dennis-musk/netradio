#ifndef CLIENT_CONF_H
#define CLIENT_CONF_H

struct client_conf_st{
	char *rcv_port;
	char *mgroup;
	char *player_cmd;
};

extern struct client_conf_st client_conf;

/* - indacate stdin or stdout */ 
//#define DEFAULT_PLAYCMD "/usr/bin/mpg123 - > /dev/null 2>&1"
#define DEFAULT_PLAYCMD "/usr/bin/mpg123 - "
#define DEFAULT_IF "eth0"

#endif
