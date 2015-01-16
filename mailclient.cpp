/*************************************************************************
    > File Name: smtp.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2014年11月18日 星期二 19时25分43秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<netdb.h>
#include<unistd.h>
#include<fstream>
#include<string>
#include<sstream>
#include"mail.pb.h"
#include"mailclient.h"
using namespace std;


int send_socket(string sendmsg,int sockfd)
{
	return  send(sockfd,sendmsg.c_str(),sendmsg.size(),0); 
}

int recv_socket(string & sRecvMsg,int sockfd)
{
	char buffer[1000];
	memset(buffer,0,sizeof(buffer));
	int len = recv(sockfd,buffer,sizeof(buffer),0);
	sRecvMsg = string(buffer,len);
	return len;
}

MailClient::MailClient(string configFile,int sockfd_tmp,int iPort_tmp)
	:sockfd(sockfd_tmp),iPort(iPort_tmp)
{
	ifstream readfile;
	readfile.open(configFile.c_str());
	if (!readfile.is_open())
	{
		printf("config file error\n");
		return;
	}
	char buf[100];
	if (!readfile.eof())
	{
		readfile.getline(buf,100);
		sIp = string(buf);
	}
	if (!readfile.eof())
	{
		readfile.getline(buf,100);
		iPort = atoi(buf);
	}
	readfile.close();
}

int MailClient::Connect()
{
	memset(&sockaddr_in_obj,0,sizeof(sockaddr_in_obj));
	sockaddr_in_obj.sin_family = AF_INET;
	sockaddr_in_obj.sin_port = htons(iPort);
	if (inet_aton(sIp.c_str(),(struct in_addr *)&sockaddr_in_obj.sin_addr.s_addr) == 0)
	{
		printf("ip error : %s\n",sIp.c_str());
		return -1;
	}
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("create socket error\n");
		return -2;
	}
	if (connect(sockfd,(struct sockaddr *) &sockaddr_in_obj,sizeof(sockaddr_in_obj))==-1)
	{
		printf("connect error\n");
		return -3;
	}
	return 0;
}

MailClient::~MailClient()
{
	close(sockfd);
}

int MailClient::FuncCheckId(const SenderMsg sendermsg_obj,CheckIdReturnMsg &checkidreturnmsg_obj)
{
	FuncType functype_obj;
	string sendermsg_obj_str;
	sendermsg_obj.SerializeToString(&sendermsg_obj_str);
	functype_obj.set_msg(sendermsg_obj_str);
	functype_obj.set_type(2);
	string sSendMsg;
	functype_obj.SerializeToString(&sSendMsg);
	int iRet = send_socket(sSendMsg,sockfd);
	if (iRet < 0)
	{
		printf("send error\n");
		return -1;
	}
	string sCheckIdReturnMsg;
	iRet = recv_socket(sCheckIdReturnMsg,sockfd);
	if (iRet > 0)
	{
		if (!checkidreturnmsg_obj.ParseFromString(sCheckIdReturnMsg))
		{
			printf("parse error\n");
			return -4;
		}
	}
	else if (iRet < 0)
	{
		return -2;
	}
	else 
	{
		printf("the other close\n");
		return -3;
	}
	return 0;
}

