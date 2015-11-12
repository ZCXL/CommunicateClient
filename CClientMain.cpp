/* CClientMain.cpp */

#include<iostream>
#include<string>

#include<stdlib.h>

#include"CClient.h"

using namespace std;

int main(int argc, char* argv[])
{
	int port;
	string svrIp;
	string user;
	CClient* client;

	if(argc == 4){
		user=string(argv[3]);
		port = atoi(argv[2]);
		svrIp=string(argv[1]);
		client = new CClient(svrIp,port,user);
	}else if(argc==2){
		user=string(argv[1]);
		client = new CClient(user);
	}else{
		cout<<"usage: <filename> [serverIp] [port] <username>"<<endl;
		return 0;
	}

	if(client->run()){
		cerr<<"Error connect to communication server"<<endl;
	}

	free(client);

	return 0;
}