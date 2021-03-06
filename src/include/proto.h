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


#define MSG_CHANNEL_MAX  (65536/* max packet */ - 20/* ip head */ - 8 /* udp head */)
//#define MSG_CHANNEL_MAX  (512/* max packet */ - 20/* ip head */ - 8 /* udp head */)

/* channel data */
struct msg_channel_st {
	chnid_t id; /* must between MINCHNID and MAXCHNID */
	uint8_t data[0];
} __attribute__((packed));

/* channel description */ 
struct msg_listentry_st {
	chnid_t id; /* must between MINCHNID and MAXCHNID */
	uint16_t len;
	uint8_t descr[0];
} __attribute__((packed));

#define MSG_LIST_MAX  (65536/* max packet */ - 20/* ip head */ - 8 /* udp head */)

/* chanel list */
struct msg_list_st {
	chnid_t id; /* must be LISTCHNID */
	struct msg_listentry_st entry[0];
} __attribute__((packed));


#endif
