#include "../common/my_define.h"
#include "../net/my_socket.h"
#include "../work/work.h"
#include "../work/my_work.h"
#include "../work/work_factory.h"
#include "../common/log.h"
#include "../common/config.h"

int main()  
{
	printf("load config file ...\n");
	Config::GetInstance()->Init("F:\\work_nc\\MyProject\\CompileOnline\\conf\\config.conf");
	Config::GetInstance()->Test();
	printf("load config file success\n");
	
	string logfile = Config::GetInstance()->GetValue("logfile");
	MyLogInstance->Init(logfile);

	WorkFactory *workFactory = new WorkFactory;
	Work *myWork  = workFactory->CreateWork(COMPILEWORK);

	string ip = Config::GetInstance()->GetValue("listenip");
	unsigned short port = atoi(Config::GetInstance()->GetValue("listenport").c_str());
	MySocket mySocket;
	int rt = mySocket.InitSocket(ip,port);
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

