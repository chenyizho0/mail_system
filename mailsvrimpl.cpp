/*************************************************************************
    > File Name: mailsvrimpl.cpp
    > Author: laixukai
    > Mail: laixukai@126.com 
    > Created Time: 2015年01月15日 星期四 00时37分02秒
 ************************************************************************/

#include<iostream>
#include"mail.pb.h"
#include"mailsvrimpl.h"
using namespace std;


int func(SenderMsg & sendmsg,CheckIdReturnMsg & checkidreturnmsg)
{
	int i = sendmsg.smtpsvrport();
	CheckIdReturnMsg c;
	c.set_error_code(i-10);
	checkidreturnmsg = c;
	return 0;
}
