#ifndef __RESPONSEHTTP_H__
#define __RESPONSEHTTP_H__

/*
	file		:	ResonpseHttp.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "Http.h"

class ResponseHttp : public Http
{
public:
	ResponseHttp();
	~ResponseHttp();
	
	void Init();
	int SetMsgBody(const string &content);
	int SetMsgHead(const string &key, const string &value);
	int SetStatus(const string &status);

	const string& ResponseContent(); 

private:
	string status;

};





#endif

