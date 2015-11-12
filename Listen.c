#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "Global.h"
#include "Info.h"
#include "Client.h"
#include "Config.h"
#include "Listen.h"

static int sockfd;
static Client* client;
void* listenToFd(void*arg){
	string funcName="listenToFd:";
	sockfd=*((int*)arg);
	ConnectState state;
	for(;;){
		client=new Client();
		if(recvData()<0){
			cerr<<funcName<<"failed to receive data from server"<<endl;
			state=SERVERERROR;
			pthread_exit((void*)state);
		}
		if(parseData()<0){
			cerr<<funcName<<"failed to parse data"<<endl;
			delete client;
			continue;
		}
		if(printData()<0){
			cerr<<funcName<<"failed to print data"<<endl;
			delete client;
			continue;
		}
		delete client;
	}
	state=QUIT;
	return ((void*)state);
}
int recvData(){
	string funcName = "recvData: ";
	int recvLength;
	char* buf = new char[BUF_SIZE];
	memset(buf, '\0', BUF_SIZE);

	if(!(recvLength = recv(sockfd, buf, BUF_SIZE, 0))){
		cerr<<funcName<<"Failed to receive data (blocking)"<<endl;
		return -1;
	}
	client->addData(buf,recvLength);
	while(1){
		memset(buf, '\0', BUF_SIZE);
		recvLength = recv(sockfd, buf, BUF_SIZE, MSG_DONTWAIT);

		if(recvLength < 0){
			if(errno == EWOULDBLOCK || errno == EAGAIN){
				break;
			} else {
				cerr<<funcName<<"Failed receiving data (nonblocking)"<<endl;
				return -1;
			}
		}
	
		client->addData(buf, recvLength);

		if(recvLength<BUF_SIZE)
			break;
	}

	return 0;
}
int parseData(void){
	string funcName="parseData:";
	if(client->parseData()<0){
		cerr<<funcName<<"failed to parse data"<<endl;
		return -1;
	}
	return 0;
}
int printData(void){
	client->printInfo();
	return 0;
}
