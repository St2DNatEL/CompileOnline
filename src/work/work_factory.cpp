#include "work_factory.h"

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
		{
			CompileWork *compilework = new CompileWork;
			compilework->InitFilePath();
			return compilework;
		}
	default:
		LOG("not suppport");
		MyLogInstance->WriteLog("not suppport");
		return NULL;
	}
}