
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <site_types.h>
#include <proto.h>

int main(void)
{
	int sd, i;
	struct msg_list_st *listbuf;
	struct msg_listentry_st *tmp;
	struct ip_mreqn mreq;
	struct sockaddr_in raddr;
	chnid_t id[3] = {12, 23, 34};
	uint8_t *descr[3] = {"music", "opera", "talks"};
	
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("sock()");
		exit(1);
	}

	inet_pton(AF_INET, "224.2.2.2", &mreq.imr_multiaddr);
	inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
	mreq.imr_ifindex =  if_nametoindex("eth0");

	/* man 7 ip */
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_IF, &mreq, sizeof(mreq)) < 0 ) { 
		perror("setsockopt()");
		exit(1);
	}   



	listbuf = malloc(28);
	if (listbuf == NULL) {
		perror("malloc");
		exit(1);
	}

	listbuf->id = LISTCHNID;
	tmp = listbuf->entry;
	for (i=0; i < 3; ++i) { 
		tmp->id = id[i];
		tmp->len = htons(9);
		tmp->descr, descr[i];	
		tmp = (void *)((char *)tmp + 9);
	}

	raddr.sin_family = AF_INET;
	raddr.sin_port = htons(1999);
	inet_pton(AF_INET, "224.2.2.2", &raddr.sin_addr);

	while (1) {
		sendto(sd, listbuf, 28, 0, (struct sockaddr *)&raddr, sizeof(raddr));
		sleep(1);
	}
}





