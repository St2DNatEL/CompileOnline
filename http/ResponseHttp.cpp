#include "ResponseHttp.h"

ResponseHttp::ResponseHttp()
{
	
}
ResponseHttp::~ResponseHttp()
{
}

void ResponseHttp::Init()
{
	mapMsgHead.insert(pair<string,string>("Server","xxh"));
	mapMsgHead.insert(pair<string,string>("Content-Type","text/html"));
	mapMsgHead.insert(pair<string,string>("Connection","close"));

	status = "200 OK";
	protocolVersoin = "HTTP/1.1";
}
int ResponseHttp::SetMsgBody(const string &content)
{
	msgBody = content;
	char clen[10];
	sprintf(clen,"%d",msgBody.length());
	string cnt = "Content-Length";
	string len = string(clen);
	mapMsgHead.insert(pair<string,string>(cnt,len));

	return RT_OK;
}
int ResponseHttp::SetMsgHead(const string &key, const string &value)
{
	map<string,string>::iterator it = mapMsgHead.find(key);
	if(it != mapMsgHead.end())
		mapMsgHead[it->first] = value;
	else
		mapMsgHead.insert(pair<string,string>(key,value));
	return RT_OK;
}

int ResponseHttp::SetStatus(const string &status)
{
	this->status = status;
	return RT_OK;
}

const string& ResponseHttp::ResponseContent()
{
	strBuf = protocolVersoin + " " + status + "\r\n";
	for(map<string,string>::iterator it = mapMsgHead.begin(); it != mapMsgHead.end(); it++)
	{
		strBuf += it->first + ": " + it->second + " \r\n";
	}
	
	strBuf += "\r\n";
	strBuf += msgBody;

	return strBuf;
}


