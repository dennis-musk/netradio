#ifndef PROTO_H
#define PROTO_H

#include <stdint.h>
#include <site_types.h>

#define DEFAULT_RCVPORT "1999"
#define DEFAULT_MGROUP 	"224.2.2.2"

#define LISTCHNID 	0

#define CHNNR 		200
#define MINCHNID 	1
#define MAXCHNID 	(MINCHNID + CHNNR - 1)

/* channel data */
struct msg_channel_st {
	chnid_t id; /* must between MINCHNID and MAXCHNID */
	uint8_t data[1];
} __attibute__((packed));

/* channel description */ 
struct msg_listentry_st {
	chnid_t id; /* must between MINCHNID and MAXCHNID */
	uint16_t len;
	uint8_t descr[1];
} __attibute__((packed));

/* chanel list */
struct msg_list_st {
	chnid_t id; /* must be LISTCHNID */
	struct msg_listentry_st entry[1];
} __attibute__((packed));


#endif
