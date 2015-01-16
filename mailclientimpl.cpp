/*************************************************************************
    > File Name: mailclientimpl.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月16日 星期五 06时50分28秒
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

int main()
{
	MailClient mailclient_obj("mailclient.xml");
	int iRet = mailclient_obj.Connect();
	if (iRet == -1)
	{
		printf("client connect error %d\n",iRet);
		return 0;
	}
	SenderMsg sendermsg_obj;
	CheckIdReturnMsg checkidreturnmsg_obj;
	sendermsg_obj.set_sendername("laixukai");
	sendermsg_obj.set_sendermailname("laixukai@126.com");
	sendermsg_obj.set_smtpsvrname("smtp.163.com");
	sendermsg_obj.set_smtpsvrport(25);
	sendermsg_obj.set_base64name("base64name");
	sendermsg_obj.set_base64passwd("base64passwd");

	iRet = mailclient_obj.FuncCheckId(sendermsg_obj,checkidreturnmsg_obj);
	if (iRet != 0)
	{
		printf("funccheckid error %d\n",iRet);
		return 0;
	}
	cout << checkidreturnmsg_obj.error_code() << endl; 
	return 0;
}
