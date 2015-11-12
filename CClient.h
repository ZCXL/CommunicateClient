#ifndef _C_CLIENT_H_
#define _C_CLIENT_H_
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <pthread.h>
#include "Config.h"
#include "Info.h"
#include "Client.h"
using namespace std;
class CClient{
	public:
		CClient(string);
		CClient(string,int,string);
		~CClient();

		int setPort(size_t );
		int initSocket(void);
		int createNewSocket(void);
		int createNewThread(void);
		
		int run(void );
		
		int produceData(void);
		int sendData(int);
		void setUser(string);
		string getUser(void);
		string getTime(void);
	private:
		int sockfd,newsockfd;
		pthread_t tid;
		struct sockaddr_in servAddr,sendAddr;
		Info *info;
		Client *client;
		size_t svrPort;
		string user;
		bool isLogin;
		
		void *threadstate;
		string svrIp;
}; /* _C_CLIENT_H_ */
#endif