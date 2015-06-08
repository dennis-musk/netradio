#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#include <proto.h>
#include "thr_list.h"
#include "server_conf.h"
#include "server.h"

static pthread_t tid_list;

static struct mlib_listentry_st *list_ent;
static int nr_list_ent;

static void *thr_list(void *unused)
{
	int i, size, totalsize, ret;
	struct msg_listentry_st *entryp;
	struct msg_list_st *entlistp;

	totalsize = sizeof(chnid_t);

	for (i = 0; i < nr_list_ent; ++i) {
		printf("desc is %s\n", list_ent[i].desc);
		totalsize += sizeof(struct msg_listentry_st) + strlen(list_ent[i].desc) + 1;
		printf("total is %d\n", totalsize);
	}


	entlistp = malloc(totalsize);
	if (entlistp == NULL) {
		syslog(LOG_ERR, "malloc(): %m");
		exit(1);
	}

	entlistp->id = LISTCHNID;
	entryp = entlistp->entry;

	for (i = 0; i < nr_list_ent; ++i) {
		size = sizeof(struct msg_listentry_st) + strlen(list_ent[i].desc) + 1;
		entryp->id = list_ent[i].id;
		entryp->len = htons(size);
		strcpy((char *)entryp->descr, list_ent[i].desc);

		entryp = (void *)((char *)entryp + size);
	}


	while (1) {
		ret = sendto(serversd, entlistp, totalsize, 0,(void *)&sndaddr, sizeof(sndaddr));
		if (ret < 0) {
			syslog(LOG_WARNING, "sendto(): %m");
		} else {
			syslog(LOG_DEBUG, "list send success.");
		}

		sleep(1);	
	}
}

int thr_list_create(struct mlib_listentry_st *listp, int nr_ent)
{
	int err;

	list_ent = listp;
	nr_list_ent = nr_ent;

	err = pthread_create(&tid_list, NULL, thr_list, NULL);
	if (err) {
		syslog(LOG_ERR, "pthread_create(): %m");
		return -err;
	}

	return 0;
}

int thr_list_destroy(void)
{
	pthread_cancel(tid_list);
	pthread_join(tid_list, NULL);

	return 0;
}
