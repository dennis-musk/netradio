#ifndef MYTBF_H
#define MYTBF_H

typedef void mytbf_t;

mytbf_t *mytbf_init(int cps, int burst);

int mytbf_destroy(mytbf_t *);

int mytbf_fetchtoken(mytbf_t *, int n);

void mytbf_returntoken(mytbf_t *, int n);


#endif
