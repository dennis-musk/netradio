#include <stdio.h>
#include <net/if.h>

#include <proto.h>

static int pd[2];

struct client_conf_st client_conf = {
	.rcv_port = DEFAULT_RCVPORT,
	.mgroup = DEFAULT_MGROUP,
	.player_cmd = DEFAULT_PLAYCMD
};

static void printhelp (void)
{

}


/*
 *  -P, --port  receive port
 *  -M, --mgroup  multipe group
 *  -p, --play  play name
 *  -H, --hlep help
 */

struct option argarry[] = {
	{"port", 1, 'P'},
	{"mgroup", 1, 'M'},
	{"play", 1, 'p'},
	{"help", 0, NULL}
};

#define NR_ARGS 4

int main(int argc, char **argv)
{
	pid_t pid;
	int c;
	int sd;
	ssize_t len;
	struct sockaddr_in laddr, raddr, serveraddr;
	socklen_t raddr_len, serveraddr_len;
	struct msg_list_st *msg_list;
	struct msg_channl_st *msg_channel;
	chnid_t chosenid;
	struct ip_mreqn mreq;

	/* initialize */
	/* parse conf file
	 * parse environment
	 * parse command line
	 */
	while (1) {
		/* have argument need a ":" */
		c = getopt_long(arc, argv, "P:M:p:H",
				argarry, NR_ARGS);
		if (c < 0) {
			break;
		}
		switch (c) {
			case 'P':
				client_conf.rcv_port = optarg;
				break;
			case 'M':
				client_conf.mgroup = optarg;
				break;
			case 'p':
				client_conf.player_cmd = optarg;
				break;
			case 'H':
				printhelp();
				exit(0);
			default:
				abort();
		}
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/);
	if (sd < 0) {
		perror("socket()");
		exit(1);
	}

	inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
	mreq.imr_ifindex =  if_nametoindex("eth0");

	/* man 7 ip */
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0 ) {
		perror("setsockopt()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(client_conf.rcv_port));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);	

	if (bind(sd, (struct sockaddr *)&laddr, sizeof(laddr)) < 0) {
		perror("bind()");
		exit(1);
	}

	if (pipe[pd] < 0) {
		perror("pipe()");
		exit(1);
	}

	pid = fork();
	if (pid < 0) {
		perror("fork()");
		exit(1);
	} else (pid == 0) {
		close(pd[1]); /* close write */
		dup2(pd[0], 0);
		if (pd[0] > 0) {
			close(pd[0]);
		}

		/* child, run decorder */
		execl("/bin/sh", "sh", "-c", client_conf.player_cmd,  NULL);
		exit(0);
	}

	/* parent */
	close(pd[0]);

	msg_list = malloc(MSG_LIST_MAX);
	if (msg_list == NULL) {
		perror("malloc()");
		exit(1);
	}

	raddr_len = sizeof(raddr);

	/* receive menulist */
	while (1) {
		len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (struct sockaddr *)&raddr, &raddr_len);
		if (len < sizeof(struct msg_list_st)) {
			continue;
		}
		if (msg_list->id != LISTCHNID) { /* one bye, no need order convert */
			continue;
		}

		break;
	}

	/* select channel */
	chosenid = 5;/* assume id = 5 */

	free(msg_list);

	msg_channel = malloc(MSG_CHANNEL_MAX);
	if (msg_channel == NULL) {
		perror("malloc()");
		exit(1);
	}

	while (1) {
		len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (struct sockaddr *)&raddr, &raddr_len);
		if (raddr.sin_addr.s_addr != serveraddr.sin_addr.s_addr || raddr.sin_port != serveraddr.sin_port) {
			continue;
		}
		if (len < sizeof(struct msg_channel_st)) {
			continue;
		}
		if (msg_channel->id == chosenid) {
			/* play */
			writen(pd[1], msg_channel->data, len - sizeof(struct msg_channel_st) - 1);
		}
	}

	free(msg_channel);

	close(sd);

	exit(0);
}
