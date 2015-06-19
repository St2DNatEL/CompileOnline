#ifndef __WORK_FACTORY_H__
#define __WORK_FACTORY_H__

#include "Work.h"
#include "MyWork.h"
#include "CompileWork.h"
#include "../common/MyDef.h"

class WorkFactory
{
public:
	WorkFactory();
	~WorkFactory();

	Work* CreateWork(WORKTYPE worktype);
};





#endif
