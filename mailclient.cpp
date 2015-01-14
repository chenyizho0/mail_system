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
using namespace std;
#define MAXBUF 1024

int PORT;
int sockfd;
struct sockaddr_in dest;

char buffer[MAXBUF + 1];

char smtpSvrName[100];

char username[100];

char userMailName[100];

char base64Username[100];

char base64Password[100];

char recvMailName[100];

string subject;

string content;

int readconfig(const char * filename)
{
	fstream readfile(filename);
	if (!readfile.is_open())
	{
		printf("file %s is bad\n",filename);
		return -1;
	}
	
	if (!readfile.eof())
	{
		readfile.getline(userMailName,100);
	}
	else
	{
		return -1;
	}
	int i;
	for (i = 0;i < strlen(userMailName) && i < 100;i++)
	{
		if (userMailName[i] != '@')
			username[i] = userMailName[i];
		else
			break;
	}
	if (i == strlen(userMailName))
		return -1;
	else
		username[i] = '\0';
	
	if (!readfile.eof())
	{
		readfile.getline(smtpSvrName,100);
	}
	else 
	{
		return -1;
	}
	char sPort[100];
	if (!readfile.eof())
	{
		readfile.getline(sPort,100);
		PORT = atoi(sPort);
	}
	else
	{
		return -1;
	}
	if (PORT <= 0)
	{
		return -2;
	}
	if (!readfile.eof())
	{
		readfile.getline(base64Username,100);
	}
	else
	{
		return -1;
	}
	if (!readfile.eof())
	{
		readfile.getline(base64Password,100);
	}
	else
	{
		return -1;
	}
	readfile.close();
	return 0;
}

int readMail(const char * mailname)
{
	fstream readfile(mailname);
	if (!readfile.is_open())
	{
		printf("file %s is bad\n",mailname);
		return -1;
	}
	char buf[1000];
	string mailmsg;
	 //std::ifstream in("some.file"); 
	std::ostringstream tmp; 
	tmp << readfile.rdbuf(); 
	mailmsg = tmp.str();
	readfile.close();
	int startpos = -1;
	int endpos = -1;
	startpos = mailmsg.find("<to>");
	endpos = mailmsg.find("</to>");
	if (startpos < 0 || endpos < 0 || startpos + 4 > endpos)
	{
		printf("no recv mail address\n");
		return -1;
	}
	else
	{
		snprintf(recvMailName,100,mailmsg.substr(startpos+4,endpos-startpos-4).c_str());
	}
	startpos = mailmsg.find("<subject>");
	endpos = mailmsg.find("</subject>");
	if (startpos < 0 || endpos < 0 || startpos + 9 > endpos)
	{
		printf("no subject\n");
		return -1;
	}
	else
	{
		subject = mailmsg.substr(startpos+9,endpos-startpos-9); 
	}
	startpos = mailmsg.find("<content>");
	endpos = mailmsg.find("</content>");
	if (startpos < 0 || endpos < 0 || startpos + 9 > endpos)
	{
		printf("no content\n");
		return -1;
	}
	else
	{
		content = mailmsg.substr(startpos+9,endpos-startpos-9); 
	}
	return 0;

}

int send_socket(string sendmsg,int sockfd)
{
	return  send(sockfd,sendmsg.c_str(),sendmsg.size(),0); 
}

int recv_socket(string & sRecvMsg,int sockfd)
{
	memset(buffer,0,sizeof(buffer));
	int len = recv(sockfd,buffer,sizeof(buffer),0);
	sRecvMsg = string(buffer);
	return len;
}



int main(int argc,char ** argv)
{
	readconfig("user.conf");
	readMail("mail.eml");

	SenderMsg sendmsg;
	sendmsg.set_sendername(string(username));
	sendmsg.set_sendermailname(string(userMailName));
	sendmsg.set_smtpsvrname(string(smtpSvrName));
	sendmsg.set_smtpsvrport(PORT);
	sendmsg.set_base64name(string(base64Username));
	sendmsg.set_base64passwd(string(base64Password));
	string sSendmsg;
	sendmsg.SerializeToString(&sSendmsg);
	int len;
	memset(&dest,0,sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(22222);
	if (inet_aton("127.0.0.1",(struct in_addr *)&dest.sin_addr.s_addr) == 0)
	{
		perror("127.0.0.1");
		exit(errno);
	}
	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Socket");
		exit(errno);
	}
	if (connect(sockfd,(struct sockaddr *) &dest,sizeof(dest))==-1)
	{
		perror("Connect ");
		exit(errno);
	}
	int iRet = send_socket(sSendmsg,sockfd);
	
	if (iRet < 0)
	{
		printf("send error\n");
	}
	string sCheckIdReturnMsg;
	iRet = recv_socket(sCheckIdReturnMsg,sockfd);
	if (iRet > 0)
	{
		CheckIdReturnMsg checkidreturnmsg;
		if (checkidreturnmsg.ParseFromString(sCheckIdReturnMsg))
		{
			cout << checkidreturnmsg.error_code() << endl; 
		}
		else
		{
			printf("parse error\n");
		}
	}
	else if (iRet < 0)
	{
		printf("error\n");
	}
	else 
	{
		printf("the other close\n");
	}
	close(sockfd);

	return 0;
}
