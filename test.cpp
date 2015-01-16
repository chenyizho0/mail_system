/*************************************************************************
    > File Name: test.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月16日 星期五 08时33分38秒
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

	SenderMsg sendermsg_obj;
	CheckIdReturnMsg checkidreturnmsg_obj;
	sendermsg_obj.set_sendername("laixukai");
	sendermsg_obj.set_sendermailname("laixukai@126.com");
	sendermsg_obj.set_smtpsvrname("smtp.163.com");
	sendermsg_obj.set_smtpsvrport(25);
	sendermsg_obj.set_base64name("base64name");
	sendermsg_obj.set_base64passwd("base64passwd");

	FuncType functype_obj;
	string sendermsg_obj_str;
	sendermsg_obj.SerializeToString(&sendermsg_obj_str);

	functype_obj.set_msg(sendermsg_obj_str);
	functype_obj.set_type(2);	
	string sSendMsg;
	functype_obj.SerializeToString(&sSendMsg);

	FuncType functype_obj1;
	if (!functype_obj1.ParseFromString(sSendMsg))
	{
		cout << "1 errro" << endl;
	}
	cout << functype_obj1.type() << endl;
	string aa = functype_obj1.msg();
	SenderMsg sendermsg_obj1;
	if (!sendermsg_obj1.ParseFromString(aa))
	{
		cout << "2 error" << endl;
	}
	cout << sendermsg_obj1.base64name() << endl;
}

