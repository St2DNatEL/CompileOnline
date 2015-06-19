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

int MacToDos(const char *filePath)
{
	FILE *f;
	int c1,c2;
    f=fopen(filePath,"rb+");
    if (NULL==f) {
        printf("Can not open file [%s]!\n",filePath);
		return RT_ERR;
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