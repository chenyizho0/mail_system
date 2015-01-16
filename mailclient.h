/*************************************************************************
    > File Name: mailclient.h
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月16日 星期五 06时48分20秒
 ************************************************************************/

#include<iostream>
using namespace std;

class MailClient
{
public:
	MailClient(string configFile,int sockfd_tmp = 0,int iPort_tmp = 0);
	~MailClient();
	int Connect();
	int FuncCheckId(const SenderMsg sendermsg_obj,CheckIdReturnMsg &checkidreturnmsg_obj);
	int FuncSendMail(const MailMsg mailmsg_obj,SendMailReturnMsg &Sendmailreturnmsg_obj);
private:
	string sIp;
	int iPort;
	int sockfd;
	struct sockaddr_in sockaddr_in_obj;
};
