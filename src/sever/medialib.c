#include <stdio.h>
#include <stdlib.h>
#include <glob.h>

#include "medialib.h"
#include "server_conf.h"

#define PATHSIZE 	1024

struct channel_context_st {
	chnid_t id;
	char *desc;
	glob_t mp3glob;
	int pos;
	off_t offset;
	mytbuf_t *tbf;
};

static struct channel_context_st channel[CHNNR];

static struct channel_context_st *path2entry(const char *)
{
	struct mlib_listentry_st *me;


}

int mlib_getchnlist(struct mlib_listentry_st **result, int *resum)
{
	glob_t globres;
	char path[PATHSIZE];
	struct mlib_listentry_st *ptr;
	struct channel_context_st *res;
	int num;

	snprintf(path, PATHSIZE, "%s/*", server_conf.media_dir);

	if (glob(path, 0, NULL, &globres)) {
		return -1;
	}

	ptr = malloc(globres.gl_pathc * sizeof(struct mlib_listentry_st));
	if (ptr == NULL) {
		syslog(LOG_ERR, "malloc(): %m");
		exit(1);
	}

	num = 0;

	for (i = 0; i < globres.gl_pathc; ++i) {
		res = path2entry(globres.gl_pathv[i]);
		if (ptr[num]) {
			memcpy(channel + num, res, sizeof(*res));
			ptr[num].id = res->id;
			ptr[num].desc = strdup(res->desc);

			num++;
		}
	}

	result = realloc(ptr, num * sizeof(struct mlib_listentry_st));
	*renum = num;

	return num;
}

int mlib_freechnlist(struct mlib_listentry_st *)
{

}

ssize_t mlib_readchn(chnid_t, void *, size_t)
{

}
