#ifndef __REQUESTHTTP_H__
#define __REQUESTHTTP_H__

/*
	file		:	RequestHttp.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "Http.h"

class RequestHttp : public Http
{
public:
	RequestHttp();
	~RequestHttp(void);
	
	int Parse(const char *buf);
	const string& GetMethod();
	const string& GetMsgHeadParamValue(const string &param);
	const string& GetPath();

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
};


#endif
