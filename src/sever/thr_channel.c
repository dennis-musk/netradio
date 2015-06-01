#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "thr_channel.h"
#include "medialib.h"

struct thr_channel_entry_st {
	chnid_t chnid;
	pthread_t tid;
}

struct msg_channel_st *sbuf;
static struct thr_channel_entry_st thr_chnnel[CHNNR];
static int nextpos = 0;

static void *thr_channel(void *unused)
{
	while (1) {
		pthread_testcancel();
		mlib_readchn();
		sbuf->;
		sendto();
	}
}

int thr_channel_create(struct mlib_listentry_st *ptr)
{
	int err;
	int len;

	sbuf = malloc(MSG_CHANNEL_MAX);
	if (sbuf == NULL) {
		syslog(LOG_ERR, "malloc(): %m");
		return -ENOMEM;
	}

	if (tid_nextpos >= CHNNR) {
		free(sbuf);
		return -ENOSPC;
	}

	err = pthread_create(&thr_channel[nextpos].tid, NULL, thr_channel, NULL);
	if (err < 0) {
		syslog(LOG_WARNING, "pthread_create(): %m");
		return -err;
	}

	thr_channel[nextpos].chnid = ptr->id;

	++nextpos;

	return 0;
}

int thr_channel_destroy(struct mlib_listentry_st *ptr)
{
	int i;

	for (i = 0; i < CHNNR; ++i) {
		if (thr_channel[i].chnid == ptr->id) {
			if (pthread_cancel(thr_channel[i].id)) {
				syslog(LOG_ERR, "The thread of channel %d", ptr->id);
				return -ESRCH;
			}
			thr_channel[i].chnid = -1;
			pthread_join(thr_channel[i].tid, NULL);
			return 0;
		}
	}
	syslog(LOG_ERR, "Channel %d dosen't exsit", ptr->id);

	return -ESRCH;
}

int thr_channel_destroyall(void)
{
	for (i = 0; i < CHNNR; ++i) {
		if (thr_channel[i].chnid > 0) {
			if (pthread_cancel(thr_channel[i].id)) {
				syslog(LOG_ERR, "The thread of channel %d", ptr->id);
				return -ESRCH;
			}
			thr_channel[i].chnid = -1;
		}
	
	}

	return 0;
}

