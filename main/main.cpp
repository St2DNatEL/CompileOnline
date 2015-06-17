#include "../common/MyDef.h"
#include "../net/MySocket.h"
#include "../work/Work.h"
#include "../work/MyWork.h"
#include "../work/WorkFactory.h"


int main()  
{  
	WorkFactory *workFactory = new WorkFactory;
	Work *myWork  = workFactory->CreateWork(MYWORK);

	MySocket mySocket;
	int rt = mySocket.InitSocket("0.0.0.0",8888);
	if(rt != RT_OK)
	{
		cout << RT_INFO[rt] << endl;
		return 0;
	}

	rt = mySocket.SetWork(myWork);
	if(rt != RT_OK)
	{
		cout << RT_INFO[rt] << endl;
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

int main1()
{
	string str = "My name is xiong xian he1";
	vector<string> vc;
	Split(vc, str, "io");
	for(int i = 0; i<vc.capacity(); i++)
		cout << vc[i] << endl;
	return 0;
}
