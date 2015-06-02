#ifndef SERVER_CONF_H
#define SERVER_CONF_H

enum {
	run_daemon,
	run_foregound
};


struct server_conf_st {
	char *rcv_port;
	char *mgroup;
	char *media_dir;
	char *ifname;
	char runmode;
};

extern struct server_conf_st server_conf;


#define DEFAULT_MEDIADIR 	"/home/rxhc/Downloads/media"
#define DEFAULT_IF 	"eth0"

#endif
