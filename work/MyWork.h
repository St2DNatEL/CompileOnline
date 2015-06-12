#ifndef __MYWORK_H__
#define __MYWORK_H__

/*
	file		:	MyWork.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "Work.h"
#include "../http/Http.h"

class MyWork:public Work
{
public:
	MyWork(void);
	~MyWork(void);

	virtual int DoWork(const char *recvbuf, const int recvlen, string &sendbuf, int &sendlen);
};

#endif
