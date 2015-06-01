#include <stdio.h>
#include <stdlib.h>
#include <pthrad.h>

#include "thr_list.h"
#include "server.h"

static pthrad_t tid_list;

static void *thr_list(void *unused)
{
	while (1) {
		sendto(serversd, );
		sleep(1);	
	}

}

int thr_list_create(truct mlib_listentry_st **, int)
{
	int err;

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
	pthrad_join(tid_list, NULL);

	reurn 0;
}
