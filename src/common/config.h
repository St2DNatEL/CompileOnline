#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "my_define.h"
#include <map>


const int kTempBufferMax = 1024;

class Config
{
private:
	Config(){}
	~Config(){}
	Config& operator=(const Config& rhs){ return *this;};

public:
	static Config* GetInstance();
	void Init(const string &filename);
	string GetValue(const string &param);
	void Test();

private:
	int ReadFile();
	void ParseLine(const string &strline);
	string DeleteSpace(const string &str);
	void PaseKewValue(const string &str);

private:
	static Config *config_;
	string filename_;
	map<string,string> mapparam_;
};

#endif

