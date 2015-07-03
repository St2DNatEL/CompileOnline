#include "config.h"
#include <cstdlib>
#include <fstream>

Config* Config::config_ = NULL;

Config* Config::GetInstance()
{
	if(config_ == NULL)
		config_ = new Config;
	return config_;
}

void Config::Init(const string &filename)
{
	filename_ = filename;
	ReadFile();
}
string Config::GetValue(const string &param)
{
	map<string,string>::iterator it = mapparam_.find(param);
	if(it == mapparam_.end())
		return "";
	return it->second;
}

int Config::ReadFile()
{
#ifdef __LINUX__
	fstream fs(filename_.c_str(), ios::in);
#else
	fstream fs(filename_, ios::_Nocreate);
#endif
	if(!fs)
	{
		printf("error: Config::ParseConfig open file failed\n");
		return RT_ERR;
	}
	char readline[kTempBufferMax]={0};
	while(fs.getline(readline,kTempBufferMax))
	{
		ParseLine(readline);
	}
	fs.close();
	return RT_OK;
}

void Config::ParseLine(const string &strline)
{
	if(strline.empty())
		return;
	int pos = strline.find("#");
	if(pos == 0)
		return;
	string str = strline;
	if(pos != -1)
	{
		str = string(strline,0,pos);
	}
	str = DeleteSpace(str);

	PaseKewValue(str);
}

string Config::DeleteSpace(const string &str)
{
	if(str.empty())
		return "";
	int len = str.length();
	char *tmpbuf = new char[len+1];
	int j = -1;
	for(int i = 0; i < len; i++)
	{
		if(str[i] != ' ')
			tmpbuf[++j] = str[i];
	}
	tmpbuf[++j] = '\0';
	return string(tmpbuf);
}
void Config::PaseKewValue(const string &str)
{
	if(str.empty())
		return;
	vector<string> vc;
	Split(vc, str, "=");
	if(vc.capacity() != 2)
		return;
	mapparam_.insert(pair<string,string>(vc[0],vc[1]));
}

void Config::Test()
{
	map<string,string>::iterator it = mapparam_.begin();
	for(; it != mapparam_.end(); ++it)
		printf("[%s] -> [%s]\n", it->first.c_str(), it->second.c_str());
}