#include "my_define.h"

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

int MacToDos(const char *filePath)
{
	FILE *f;
	int c1,c2;
    f=fopen(filePath,"rb+");
    if (NULL==f) {
        printf("Can not open file [%s]!\n",filePath);
		return RT_ERR_OPEN_FILE;
    }
    while (1) {
        c1=fgetc(f);
        if (EOF==c1) break;
        if ('\r'==c1) {
            c2=fgetc(f);
            if (EOF==c2) {
                fseek(f,-1L,SEEK_CUR);
                fputc('\n',f);
                break;
            }
            if ('\n'!=c2) {
                fseek(f,-2L,SEEK_CUR);
                fputc('\n',f);
                fseek(f,1,SEEK_CUR);
            }
        }
    }
    fclose(f);
	return RT_OK;
}

string& ReplaceAll(string &orgStr, const string &toReplaceStr, const string &replaceStr)
{
	string tmp = "";
	int begin = 0;
	int pos = orgStr.find(toReplaceStr);
	if(pos == -1)
		return orgStr;
	int toStrLen = toReplaceStr.length();
	int repStrLen = replaceStr.length();

	while(pos >= 0)
	{
		tmp += string(orgStr,begin,pos-begin);
		tmp += replaceStr;
		begin = pos + toStrLen;
		pos = orgStr.find(toReplaceStr,begin);
	}
	orgStr = tmp + string(orgStr,begin);
	return orgStr;
}

