#ifndef THR_CHANEL_H
#define THR_CHANEL_H

#include "medialib.h"

int thr_channel_create(struct mlib_listentry_st *);
int thr_channel_destroy(struct mlib_listentry_st *);

int thr_channel_destroyall(void);


#endif
