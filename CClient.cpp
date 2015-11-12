#include <string>
#include <cstring>
#include <sstream>

#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <time.h>
#include <pthread.h>
#include "Config.h"
#include "Info.h"
#include "Global.h"
#include "CClient.h"
#include "Client.h"
#include "Listen.h"
using namespace std;

CClient::CClient(string u):svrPort(Port),svrIp("127.0.0.1")
{
	user=u;
	isLogin=false;
}

CClient::CClient(string ip,int port,string u)
{
	string funcName="CClient::CClient:";
	
	if(setPort(port)){
		cerr<<funcName<<"Failed to set port"<<endl;
	}
	svrIp=ip;
	user=u;
	isLogin=false;
}
CClient::~CClient()
{
	close(sockfd);
}

int CClient::setPort(size_t port){
	string funcName="CClient::setPort:";
	if(port<1024||port>65535){
		cerr<<funcName<<"Invalid port value.Connot bind.Enter a value between 1024 and 65535"<<endl;
		return -1;
	}

	svrPort=port;
	return 0;
}

int CClient::initSocket(){
	string funcName = "initSocket: ";
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		cerr<<funcName<<"Failed to create socket"<<endl;
		return -1;
	}

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(svrPort);
	servAddr.sin_addr.s_addr = inet_addr(svrIp.c_str());

	if((connect(sockfd, (struct sockaddr *)&servAddr, sizeof(servAddr)))<0){
		cerr<<funcName<<"Failed to connect"<<endl;
		return -1;
	}

	return 0;
}
int CClient::createNewSocket(){
	string funcName = "crateNewSocket: ";
	
	if((newsockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		cerr<<funcName<<"Failed to create socket"<<endl;
		return -1;
	}

	sendAddr.sin_family = AF_INET;
	sendAddr.sin_port = htons(svrPort);
	sendAddr.sin_addr.s_addr = inet_addr(svrIp.c_str());

	if((connect(newsockfd, (struct sockaddr *)&sendAddr, sizeof(sendAddr)))<0){
		cerr<<funcName<<"Failed to connect"<<endl;
		return -1;
	}

	return 0;
}

int CClient::createNewThread(){
	string funcName="createNewThread:";
	int err;
	err=pthread_create(&tid,NULL,listenToFd,(void*)&sockfd);
	if(err!=0){
		cerr<<funcName<<"failed to create new thread"<<endl;
		return -1;
	}
	return 0;	
}
int CClient::run(){
	string funcName = "run:";
	
	if(initSocket()<0){
		cerr<<funcName<<"failed to initialize socket"<<endl;
		return -1;
	}
	string instruction;
	while(1){
		cin>>instruction;
		if(instruction=="login"){
			if(isLogin){
				cout<<"Have been on line"<<endl;
				continue;
			}
			info=new Info();
			info->setUser(user);
			info->setType(CLIENT);
			if(produceData()<0){
				cerr<<funcName<<"failed to produce data"<<endl;
				return -1;
			}
			if(sendData(sockfd)<0){
				cerr<<funcName<<"failed to send data"<<endl;
				return -1;
			}
			if(createNewThread()<0){
				cerr<<funcName<<"failed to connect server"<<endl;
				return -1;
			}
			isLogin=true;
		}else if(instruction=="send"){
			if(!isLogin){
				cout<<"Be off line"<<endl;
				continue;
			}
			if(createNewSocket()<0){
				cerr<<funcName<<"failed to create new socket"<<endl;
				return -1;
			}
			string u;
			string i;
			string t;
			cin>>u>>i;
			info=new Info();
			info->setType(MESSAGE);
			info->setSender(user);
			info->setInfo(i);
			info->setReceiver(u);
			t=getTime();
			info->setTime(t);
			if(produceData()<0){
				cerr<<funcName<<"failed to produce data"<<endl;
				return -1;
			}
			if(sendData(newsockfd)<0){
				cerr<<funcName<<"failed to send data"<<endl;
				return -1;
			}
			close(newsockfd);
		}
		
	}
	return 0;
}
int CClient::produceData(){
	string funcName="produceData:";
	if(info->produceData()<0){
		cerr<<"failed to produce data"<<endl;
		return -1;
	}
	return 0;
}
int CClient::sendData(int fd){
	string funcName = "sendData: ";

	size_t dataSize = info->getDataSize();
	string* dataPtr =info->getDataPtr();

	char *buf = new char[dataSize];
	memset(buf, '\0', dataSize);
	memcpy(buf, dataPtr->data(),dataSize);

	if((send(fd, buf, dataSize, 0))<0){
		cerr<<funcName<<"Sending request failed"<<endl;
	}

	delete buf;
	delete info;
	
	return 0;
}
string CClient::getTime(){
	ostringstream stream;
	time_t nowtime=time(NULL);
	struct tm *local;
	local=localtime(&nowtime);
	stream<<local->tm_year<<"-"<<local->tm_mon<<"-"<<local->tm_mday;
	stream<<" ";
	stream<<local->tm_hour<<":"<<local->tm_min<<":"<<local->tm_sec;
	return stream.str();
}