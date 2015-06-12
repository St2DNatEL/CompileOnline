#ifndef __HTTP_H__
#define __HTTP_H__

/*
	file		:	Http.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	
*/

#include "../common/MyDef.h"
#include <map>
#include <vector>

class Http
{
public:
	Http();
	~Http(void);
	int Parse(char *buf);
	const string& GetMethod();
	const string& GetMsgHeadParamValue(const string &param);

	const string& GetQueryParamValue(const string &param);
	const string& GetClientIP();
	short GetClientPort();
	string& GetMsgBody();


private:
	int ParseRequestHead();
	int ParseMsgHead();
	int ParseMsgHeadContent();
	int ParseMsgBody();
	int ParseQueryString();

private:
	string strBuf;

	//请求头
	string requestHead;
	string method;
	string path;
	string protocolVersoin;

	//消息报头
	string msgHead;
	map<string, string> mapMsgHead;

	//消息体
	string msgBody;

	//查询字符串
	map<string, string> mapQueryString;
};

#endif
