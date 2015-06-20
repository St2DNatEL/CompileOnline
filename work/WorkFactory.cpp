#include "WorkFactory.h"

WorkFactory::WorkFactory()
{
}

WorkFactory::~WorkFactory()
{
}

Work*WorkFactory::CreateWork(WORKTYPE worktype)
{
	switch(worktype)
	{
	case MYWORK:
		return new MyWork;
	case COMPILEWORK:
		return new CompileWork;
	default:
		LOG("not suppport");
		MyLogInstance->WriteLog("not suppport");
	}
}