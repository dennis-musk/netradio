#include <stdio.h>

#include <proto.h>


static int pd[2];

/*
 *  -P receive port
 *  -M multipe group
 *  -p play name
 *  -H help
 */
int main(int argc, char **argv)
{
	pid_t pid;

	/* initialize */
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
		execl("/usr/bin/mpg123", "mpg123", "-"/* stdin/stdout */, NULL);
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
