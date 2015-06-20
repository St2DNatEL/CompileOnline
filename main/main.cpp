#include "../common/MyDef.h"
#include "../net/MySocket.h"
#include "../work/Work.h"
#include "../work/MyWork.h"
#include "../work/WorkFactory.h"
#include "../common/Log.h"

int main()  
{
	MyLogInstance->Init("F:\\work_nc\\MyProject\\CompileOnline\\log\\server.log");

	WorkFactory *workFactory = new WorkFactory;
	Work *myWork  = workFactory->CreateWork(COMPILEWORK);

	MySocket mySocket;
	int rt = mySocket.InitSocket("0.0.0.0",8898);
	if(rt != RT_OK)
	{
		cout << RT_INFO[rt] << endl;
		MyLogInstance->WriteLog(RT_INFO[rt].c_str());
		return 0;
	}

	rt = mySocket.SetWork(myWork);
	if(rt != RT_OK)
	{
		cout << RT_INFO[rt] << endl;
		MyLogInstance->WriteLog(RT_INFO[rt].c_str());
		return 0;
	}

	mySocket.Do();
	

	delete myWork;
	myWork = NULL;

	delete workFactory;
	workFactory = NULL;
	
    system("pause");  
    return 0;  
}  

