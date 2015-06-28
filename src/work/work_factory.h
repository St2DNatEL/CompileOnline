#ifndef __WORK_FACTORY_H__
#define __WORK_FACTORY_H__

#include "work.h"
#include "my_work.h"
#include "compile_work.h"
#include "../common/my_define.h"

class WorkFactory
{
public:
	WorkFactory();
	~WorkFactory();

	Work* CreateWork(WORKTYPE worktype);
};





#endif
