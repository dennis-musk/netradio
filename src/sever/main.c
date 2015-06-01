#include <stdio.h>
#include <stdlib.h>
#include <proto.h>

#include "server.conf.h"
#include "medialib.h"

struct server_conf_st server_conf = {
	.rcv_port = DEFAULT_RCVPORT;
	.mgroup = DEFAULT_MGROUP;
	.media_dir = DEFAULT_MEDIADIR;
	.ifname = DEFAULT_IF;
	.runmode = run_daemon;
};

int serversd;

static struct mlib_listentry_st *list;
static int list_size;

static void daemon_exit(int s)
{
	thr_channel_destroyall();
	thr_list_destroy();
	mlib_freechnlist(list);
	if (s < 0) {
		syslog(LOG_ERR, "Daemon failure exit.")
		exit(1);
	}
	syslog(LOG_ERR, "Signal %d exit.", s);
	closelog();
	exit(0);
}

static void daemonize(void)
{
	int fd;
	pid_t pid;

	/* keep not leader */
	pid = fork();
	if (pd > 0) {
		exit(0);
	}

	/* child is not session and group leader */
	fd = open("/dev/null", O_RDWR);
	if (fd < 0) {
		syslog(LOG_ERR, "open(): %m");
		exit(1);
	}

	dup2(fd, 0);
	dup2(fd, 1);
	dup2(fd, 2);

	if (fd > 2) 
		close(fd);

	setsid();

	syslog(LOG_INFO, "Daemon initialized OK");
	/* prevent deivice busy */
	chdir("/");
	umask(0);

	return;
}


static int socket_init()
{
	struct ip_mreqn mreq;

	serversd = socket(AF_INET, SOCK_DGRAM, 0);
	if (serversd < 0) {
		syslog(LOG_ERR, "socket(): %m");
		exit(1);
	}
	
	inet_pton(AF_INET, server_conf.mgroup, &mreq.imr_multiaddr); 
	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address); 
	mreq.imr_ifindex = if_nametoindex(server_conf.ifname);
	if (setsockopt(serversd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0) {
		syslog(LOG_ERR, "setsockopt(): %m");
		exit(1);
	}	


}


/*
 * -M MGROUP
 * -P RCVPORT
 * -D MEDIA DIR
 * -I NIC NAME
 * -F RUN foreground
 * -f log facility
 * -H HELP
 *
 */

int main(int argc, char **argv)
{
	int c;

	signal(SIGTERM, daemon_exit);
	signal(SIGINT, daemon_exit);
	signal(SIGQUIT, daemon_exit);

	openlog("netradio", LOG_PID|LOG_PERROR, LOG_DAEMON);

	/* initialize */
	while (1) {
		c = getopt(argc, argv, "M:P:D:FH");
		if (c < 0) {
			break;
		}

		switch (c) {
			case 'M':
				server_conf.mgroup = optarg;
				break;
			case 'P':
				server_conf.rcvport = optarg;
				break;
			case 'D':
				server_conf.media_dir = optarg;
				break;
			case 'F':
				server_conf.runmode = optarg;

				break;
			case 'I':
				server_conf.ifname = optarg;
				break;
			case 'H':
				break;
			default:
				exit(0);
		}
	}

	if (server_conf.runmode == run_daemon) {
		daemonize();
	} else if (server_conf.runmode == run_foregound) {
		/* do nothing */
	} else {
		syslog(LOG_ERR, "Invalid server_conf.runmode.");
		exit(1);
	}

	socket_init();

	err = mlib_getchnlist(&list, &list_size);
	if (err) {
		syslog(LOG_ERR, "mlib_getchnlist(): %m");
		exit(1);
	}

	thr_list_create(list, list_size);

	for (i = 0; i < list_size; ++i) {
		thr_channel_create(list + i);
	}

	while(1)
		pause();


	exit(0);
}
