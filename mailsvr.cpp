/*************************************************************************
    > File Name: mailsvr.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月14日 星期三 22时49分48秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
#include"mail.pb.h"
#include"mailsvrimpl.h"
using namespace std;

int myport = 22222;
int lisnum = 5;


void handler(int sig)
{
	printf("sig=%d\n",sig);
}

int main()
{
	int sockfd,new_fd;
	socklen_t socklen;
	struct sockaddr_in my_addr,their_addr;
	socklen = sizeof(their_addr);

	signal(SIGPIPE,handler);
	signal(SIGCHLD,SIG_IGN);

	if ((sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(errno);
	}
	memset(&my_addr,0,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(myport);
	my_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	if (bind(sockfd,(struct sockaddr *) &my_addr,sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(errno);
	}
	if (listen(sockfd,lisnum) == -1)
	{
		perror("listen");
		exit(errno);
  	}
	while(true)
	{
		if ((new_fd = accept(sockfd,(struct sockaddr *)&their_addr,&socklen)) == -1)
	  	{
	  		perror("accept");
	  		exit(errno);
	  		return -1;
	  	}
		int pid;
		if((pid = fork()) == -1)
		{
			perror("fork");
			close(new_fd);
			continue;
		}
		if (pid > 0)
		{
			close(new_fd);
			continue;
		}
		else if(pid == 0)
		{
			close(sockfd);
			char buffer[1000];
	  		int numbytes = recv(new_fd,buffer,1000,0);
			if (numbytes < 0)
			{
				printf("recv error\n");
				return -5;
			}
			string sFuncType = string(buffer,numbytes);
			FuncType functype_obj;
			if (!functype_obj.ParseFromString(sFuncType))
			{
				printf("parse functype error\n");
				return -2;
			}
			string sMsg;
			int iType = 0;
			sMsg = functype_obj.msg();
			iType = functype_obj.type();
			switch(iType)
			{
				case 2:
				{
					SenderMsg sendermsg;
					if (!sendermsg.ParseFromString(sMsg))
					{
						printf("parse error\n");
						return -2;
					}
					CheckIdReturnMsg checkidreturnmsg;
					int iRet = func(sendermsg,checkidreturnmsg);
					if (iRet != 0)
					{
						printf("logic error\n");
						return -3;
					}
					string sCheckidreturnmsg;
					checkidreturnmsg.SerializeToString(&sCheckidreturnmsg);
					send(new_fd,sCheckidreturnmsg.c_str(),sCheckidreturnmsg.size(),0);
					break;
				}
				default:
					break;
			}

			close(new_fd);
			exit(0);
		}
	}
}
