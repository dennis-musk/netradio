#include <stdio.h>

#include <proto.h>

static int pd[2];

struct client_conf_st client_conf = {
	.rcvport = DEFAULT_RCVPORT,
	.mgroup = DEFAULT_MGROUP,
	.player_cmd = DEFAULT_PLAYCMD
};

static void printhelp (void)
{

}

/*
 *  -P receive port
 *  -M multipe group
 *  -p play name
 *  -H help
 */
int main(int argc, char **argv)
{
	pid_t pid;
	int c;

	/* initialize */
	/* parse configure file
	 * parse environment
	 * parse command line
	 */
	while (1) {
		/* have argument need a ":" */
		c = getopt(arc, argv, "P:M:p:H");
		if (c < 0) {
			break;
		}
		switch (c) {
			case 'P':
				client_conf.rcvport = optarg;
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

	/* receive menulist */
	while (1) {
		if ("is menulist?") {
			break;
		}
	}

	/* select channel */
	while (1) {
		if ("is the chanel you want ?") {
			/* play */
			write(pd[1],);
		}
	}


	exit(0);
}
