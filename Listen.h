#ifndef _LISTEN_H_
#define _LISTEN_H_

extern void* listenToFd(void*);
extern int recvData(void);
extern int parseData(void);
extern int printData(void);

#endif