#ifndef MEDIALIB_H
#define MEDIALIB_H

#include <site_types.h>

#define MP3_PARTERN "*.mp3"
#define DESC_FNAME  "desc.text"

struct mlib_listentry_st {
	chnid_t id;
	char *desc;
};


int mlib_getchnlist(struct mlib_listentry_st **, int *);
int mlib_freechnlist(struct mlib_listentry_st *);

ssize_t mlib_readchn(chnid_t, void *, size_t);


#endif

