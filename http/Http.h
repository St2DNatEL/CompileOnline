#ifndef __HTTP_H__
#define __HTTP_H__

/*
	file		:	Http.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
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

	//����ͷ
	string requestHead;
	string method;
	string path;
	string protocolVersoin;

	//��Ϣ��ͷ
	string msgHead;
	map<string, string> mapMsgHead;

	//��Ϣ��
	string msgBody;

	//��ѯ�ַ���
	map<string, string> mapQueryString;
};

#endif
