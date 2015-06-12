#include "MyDef.h"

int Split(vector<string> &vc, const string &str, const string &sp)
{
	string tmp;
	int strlen = str.length();
	int j = 0;
	for(int i = 0; i < strlen; i++)
	{
		tmp.push_back(str[i]);
		if(str[i] == sp[0])
		{
			int ip = i;
			for(j=0; j < sp.length();j++,ip++)
			{
				if(str[ip] != sp[j]) break;
			}
			if(j == sp.length())
			{
				tmp.pop_back();
				vc.push_back(tmp);
				tmp.clear();
				i += j-1;
			}
		}
	}
	if(!tmp.empty())
	{
		vc.push_back(tmp);
	}
	return RT_OK;
}