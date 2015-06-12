#include "Http.h"

Http::Http()
{
}
Http::~Http(void)
{
}

int Http::Parse(char *buf)
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
	LOG(requestHead.c_str(),__FILE__,__LINE__);
	LOG(msgHead.c_str(),__FILE__,__LINE__);

	return RT_OK;
}

int Http::ParseRequestHead()
{
	int posHead = strBuf.find("\r\n");
	if(posHead < 0)
	{
		LOG("Http::Parse error",__FILE__,__LINE__);
		return RT_ERR;
	}
	requestHead = string(strBuf,0,posHead);

	vector<string> vc;
	Split(vc, requestHead, " ");

	if(vc.capacity() < 3)
	{
		LOG("Http::ParseRequestHead error",__FILE__,__LINE__);
		return RT_ERR_PARSE;
	}
	method = vc[0];
	path = vc[1];
	protocolVersoin = vc[2];
	return RT_OK;
}

int Http::ParseMsgHead()
{
	int msgHeadBegin = strBuf.find("\r\n");
	if(msgHeadBegin < 0)
	{
		LOG("Http::ParseMsgHead error",__FILE__,__LINE__);
		return RT_ERR;
	}

	int msgHeadEnd = strBuf.find("\r\n\r\n");
	if(msgHeadEnd < 0)
	{
		LOG("Http::ParseMsgHead error",__FILE__,__LINE__);
		return RT_ERR;
	}

	msgHead = string(strBuf.c_str()+msgHeadBegin+2, msgHeadEnd-msgHeadBegin);
	return RT_OK;
}
int Http::ParseMsgHeadContent()
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

int Http::ParseMsgBody()
{
	if(method == "POST")
	{
		string strBodyLength = mapMsgHead.at("Content-Length");
		if(strBodyLength.empty())
			return RT_OK;
		int iBodyLenth = atoi(strBodyLength.c_str());

		int pos = strBuf.find("\r\n\r\n");
		msgBody = string(strBuf.c_str()+pos+4);
	}
	return RT_OK;
}

const string& Http::GetMethod()
{
	return method;
}

const string& Http::GetMsgHeadParamValue(const string &param)
{
	map<string,string>::iterator it = mapMsgHead.find(param);
	if(it == mapMsgHead.end())
		return RT_INFO[RT_ERR];
	
	return it->second;
}

int Http::ParseQueryString()
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

const string& Http::GetQueryParamValue(const string &param)
{
	map<string,string>::iterator it = mapQueryString.find(param);
	if(it == mapQueryString.end())
		return RT_INFO[RT_ERR];
	
	return it->second;
}