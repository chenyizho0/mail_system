/*************************************************************************
    > File Name: mailclient.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月12日 星期一 04时33分26秒
 ************************************************************************/

#include<iostream>
#include"mail.pb.h"
using namespace std;

int main()
{
	SenderMsg sendmsg;
	sendmsg.set_sendername("laixukai");
	sendmsg.set_sendermailname("laixukai@163.com");
	sendmsg.set_smtpsvrname("smtp.163.com");
	sendmsg.set_smtpsvrport(25);
	sendmsg.set_base64name("bGFpeHVrYWk=");
	sendmsg.set_base64passwd("MDAxODkzMTExNDA=");
	string sSendmsg;
	sendmsg.SerializeToString(&sSendmsg);
	SenderMsg tmpmsg;
	if (tmpmsg.ParseFromString(sSendmsg))
	{
		cout << tmpmsg.sendername() << endl;
	}
	else
	{
		cout << "error" << endl;
	}
}


