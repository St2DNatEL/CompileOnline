#ifndef __WORK_H__
#define __WORK_H__

/*
	file		:	work.h
	Description	:	

	Version		:	v1.0
	Author		:	xiongxianhe@163.com & qq:360765409
	GitHub		:	https://github.com/xiongxianhe/CompileOnline
	
*/

#include "../common/my_define.h"

class Work
{
public:
	Work(void);
	virtual ~Work(void);

	virtual int DoWork(const char *recvbuf, const int recvlen, string &sendbuf, int &sendlen);
};

#endif