#include "work.h"

Work::Work(void)
{
}

Work::~Work(void)
{
}

int Work::DoWork(const char * recvbuf,const int recvlen,string &sendbuf,int & sendlen)
{
	sendbuf = string("<html><head></head><body>hello</body></html>");
	sendlen = strlen(sendbuf.c_str());
	return RT_OK;
}

