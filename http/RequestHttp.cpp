#include "RequestHttp.h"

RequestHttp::RequestHttp()
{
}
RequestHttp::~RequestHttp(void)
{
}

int RequestHttp::Parse(const char *buf)
{
	strBuf = string(buf);
	
	int rt =ParseRequestHead();
	if(rt != RT_OK)
		return rt;

	rt = ParseMsgHead();
	if(rt != RT_OK)
		return rt;

	rt = ParseMsgHeadContent();
	if(rt != RT_OK)
		return rt;

	rt = ParseMsgBody();
	if(rt != RT_OK)
		return rt;

	ParseQueryString();
	LOG(requestHead.c_str());
	LOG(msgHead.c_str());

	MyLogInstance->WriteLog(requestHead.c_str());
	MyLogInstance->WriteLog(msgHead.c_str());

	return RT_OK;
}

int RequestHttp::ParseRequestHead()
{
	int posHead = strBuf.find("\r\n");
	if(posHead < 0)
	{
		LOG("RequestHttp::Parse error");
		MyLogInstance->WriteLog("RequestHttp::Parse error");
		return RT_ERR;
	}
	requestHead = string(strBuf,0,posHead);

	vector<string> vc;
	Split(vc, requestHead, " ");

	if(vc.capacity() < 3)
	{
		LOG("RequestHttp::ParseRequestHead error");
		MyLogInstance->WriteLog("RequestHttp::ParseRequestHead error");
		return RT_ERR_PARSE;
	}
	method = vc[0];
	path = vc[1];
	protocolVersoin = vc[2];
	return RT_OK;
}

const string& RequestHttp::GetPath()
{
	return path;
}
int RequestHttp::ParseMsgHead()
{
	int msgHeadBegin = strBuf.find("\r\n");
	if(msgHeadBegin < 0)
	{
		LOG("RequestHttp::ParseMsgHead error");
		MyLogInstance->WriteLog("RequestHttp::ParseMsgHead error");
		return RT_ERR;
	}

	int msgHeadEnd = strBuf.find("\r\n\r\n");
	if(msgHeadEnd < 0)
	{
		LOG("RequestHttp::ParseMsgHead error");
		MyLogInstance->WriteLog("RequestHttp::ParseMsgHead error");
		return RT_ERR;
	}

	msgHead = string(strBuf.c_str()+msgHeadBegin+2, msgHeadEnd-msgHeadBegin);
	return RT_OK;
}
int RequestHttp::ParseMsgHeadContent()
{
	int pos = msgHead.find("\r\n");
	int beginFind = 0;
	int lineLen = pos - beginFind;
	while(pos >= 0)
	{
		vector<string> vc;
		string tmp = string(msgHead.c_str()+beginFind, lineLen);
		Split(vc,tmp,": ");
		if(vc.capacity() == 2)
		{
			mapMsgHead.insert(pair<string,string>(vc[0],vc[1]));
		}

		beginFind = pos + 2;
		pos = msgHead.find("\r\n",beginFind);
		lineLen = pos - beginFind;
	}
	return RT_OK;
}

int RequestHttp::ParseMsgBody()
{
	if(method == "POST")
	{
		string strBodyLength = mapMsgHead.at("Content-Length");
		if(strBodyLength.empty())
			return RT_OK;
		int iBodyLenth = atoi(strBodyLength.c_str());

		int pos = strBuf.find("\r\n\r\n");
		msgBody = string(strBuf.c_str()+pos+4,iBodyLenth);
	}
	return RT_OK;
}

const string& RequestHttp::GetMethod()
{
	return method;
}

const string& RequestHttp::GetMsgHeadParamValue(const string &param)
{
	map<string,string>::iterator it = mapMsgHead.find(param);
	if(it == mapMsgHead.end())
		return RT_INFO[RT_ERR];
	
	return it->second;
}

int RequestHttp::ParseQueryString()
{
	if(method == "POST")
	{
		vector<string> vc;
		
		Split(vc,msgBody,"&");
		for(vector<string>::iterator it=vc.begin(); it != vc.end();it++)
		{
			vector<string> vcparam;
			Split(vcparam, *it, "=");
			if(vcparam.capacity() == 2)
			{
				mapQueryString.insert(pair<string,string>(vcparam[0],vcparam[1]));
			}
		}
	}
	return RT_OK;
}

const string& RequestHttp::GetQueryParamValue(const string &param)
{
	map<string,string>::iterator it = mapQueryString.find(param);
	if(it == mapQueryString.end())
		return RT_INFO[RT_ERR];

	return it->second;
}

string& RequestHttp::GetMsgBody()
{
	return msgBody;
}

