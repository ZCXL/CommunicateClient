/*Client.cpp*/
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "Client.h"
#include "Info.h"
Client::Client()
{
}
Client::~Client()
{
	if(info!=NULL)
		delete info;
}
void Client::addData(const char*buf,const int &len){
	data.append(buf,len);
}

int Client::parseData(){
	string funcName="parseRequest:";
	info=new Info();
	if((info->parseData(data))<0){
		cerr<<funcName<<"failed to parse request"<<endl;
		return -1;
	}
	return 0;
}

int Client::printInfo(){
	string funcName="prepareRequest:";
	cout<<"From:"<<info->getSender()<<" "<<info->getTime()<<endl<<"	"<<info->getInfo()<<endl;
	return 0;
}