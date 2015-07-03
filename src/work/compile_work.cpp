#include "compile_work.h"
#include "../common/config.h"
#include <fstream>
#include <cstdlib>

CompileWork::CompileWork()
{
	this->cppPath = "F://";
	this->cppName = "compile.cpp";
	this->compileResultPath = "F://";
	this->compileResultName = "compile.rs";
	this->exePath = "F://";
	this->exeName = "compile.exe";
	this->exeResult = "exe.rs";

	textAreaValue = "please input code...";

	exePathName = exePath + exeName;
	cppPathName = cppPath + cppName;
	compileResultPathName = compileResultPath + compileResultName;
	exeResultPathName = exePath + exeResult;

	requestHttp = new RequestHttp;
	responseHttp = new ResponseHttp;
}
CompileWork::~CompileWork()
{
	cout << "\n++++++++\n";
	MyLogInstance->WriteLog("++++++++");
	delete requestHttp;
	delete responseHttp;
}

int CompileWork::ResponseStaticPage(string &respMsg)
{
	char bf;
	string tmp = "";
	respMsg = "";
	string path = requestHttp->GetPath();
	if(path.empty() || path == "/")
	{
		path = "index.html";
	}

	int dotpos = path.rfind(".");
	if(dotpos >= 0)
	{
		string suffix = string(path,dotpos+1);
		if(suffix == "css")
			responseHttp->SetMsgHead("Content-Type","text/css");
		else if(suffix == "js")
			responseHttp->SetMsgHead("Content-Type","application/x-javascript; charset=utf-8");
	}
	string compileworkdir = Config::GetInstance()->GetValue("COMPILEWOKRDIR");
	if(compileworkdir.empty())
		compileworkdir = COMPILEWOKRDIR;
	string dir = compileworkdir + path;
	fstream tmpFile(dir.c_str(),ios::in | ios::_Nocreate);
	if(tmpFile.is_open())
	{
		while(tmpFile.get(bf))
		{
			respMsg += bf;
		}
		tmpFile.close();
	}
	else
	{
		ResponseNotFound(respMsg);
	}

	responseHttp->SetMsgBody(respMsg);
	respMsg = responseHttp->ResponseContent();

	return RT_OK;
}
int CompileWork::ResponseNotFound(string &respMsg)
{
	char bf;
	string compileworkdir = Config::GetInstance()->GetValue("COMPILEWOKRDIR");
	if(compileworkdir.empty())
		compileworkdir = COMPILEWOKRDIR;
	string dir = compileworkdir + "404.html";
	responseHttp->SetStatus("404 Not Found");
	respMsg = "404 not Found";
	fstream tmpFile(dir.c_str(),ios::in | ios::_Nocreate);
	if(tmpFile.is_open())
	{
		respMsg = "";
		while(tmpFile.get(bf))
		{
			respMsg += bf;
		}
	}
	tmpFile.close();

	return RT_OK;
}
int CompileWork::DoCompile(string &sendbuf)
{
#ifdef __LINUX__
	responseHttp->SetMsgHead("Content-Type","text/html;charset=utf-8");
#else
	responseHttp->SetMsgHead("Content-Type","text/html;charset=GB2312");
#endif
	responseHttp->SetMsgHead("Cache-Control","no-cache");

	if(GetTextAreaValue() != RT_OK)
	{
		sendbuf = ResponseSendBuf("<font color='red'>please input code</font>");
		return RT_OK;
	}

	if(DetectionTextArea() != RT_OK)
	{
		sendbuf = ResponseSendBuf("<font color='red'>illegal code </font>");
		return RT_OK;
	}
	
	int rt = WriteCPPToFile();
	if(rt != RT_OK)
	{
		sendbuf = ResponseSendBuf("服务器暂时不能为您提供服务");
		return RT_OK;
	}

	rt = CompileCPP();
	if(rt != RT_OK)
	{
		sendbuf = ResponseSendBuf("服务器暂时不能为您提供服务");
		return RT_OK;
	}

	rt = ReadCompileResult();
	if(rt == RT_ERR_COMPILE)
	{
		sendbuf = ResponseSendBuf("<font color='red'>"+compileInfo+"</font>");
		return RT_OK;
	}
	else if(rt != RT_OK)
	{
		sendbuf = ResponseSendBuf("服务器暂时不能为您提供服务");
		return RT_OK;
	}

	rt = ExecuteEXE();
	if(rt != RT_OK)
	{
		sendbuf = ResponseSendBuf("服务器暂时不能为您提供服务");
		return RT_OK;
	}

	rt = ReadExecuteResult();
	if(rt != RT_OK)
	{
		sendbuf = ResponseSendBuf("服务器暂时不能为您提供服务");
		return RT_OK;
	}

	sendbuf = ResponseSendBuf(exeInfo);
	return RT_OK;
}

int CompileWork::DoWork(const char *recvbuf, const int recvlen, string &sendbuf, int &sendlen)
{
	cout << "\n\n" << recvbuf << "\n\n";
	MyLogInstance->WriteLog(recvbuf);

	responseHttp->Init();
	requestHttp->Parse(recvbuf);

	string path = requestHttp->GetPath();
	int endpos = path.rfind("/");
	string cmp = string(path,endpos+1);
	if(cmp == "compile")
	{
		DoCompile(sendbuf);
		//responseHttp->SetMsgHead("Content-Type","text/html; charset=GB2312");
		//responseHttp->SetMsgBody("<font color='red'>不和你玩了，我要休息了 ^_^</br>可加入 qq群讨论:233718417</font");
		//sendbuf = responseHttp->ResponseContent();
		return RT_OK;
	}
	else if(cmp == "tb")
	{
		responseHttp->SetStatus("302 ");
		responseHttp->SetMsgHead("Location", "http://www.taobao.com");
		responseHttp->SetMsgBody("");
		sendbuf = responseHttp->ResponseContent();
		return RT_OK;
	}

	
	ResponseStaticPage(sendbuf);
	return RT_OK;
}

void CompileWork::InitFilePath()
{
	string cppPath = Config::GetInstance()->GetValue("cpppath");
	string cppName = Config::GetInstance()->GetValue("cppname");
	SetCppPathName(cppPath,cppName);

	string compileResultPath = Config::GetInstance()->GetValue("compileresultpath");
	string compileResultName = Config::GetInstance()->GetValue("compileresultname");
	SetCompileResultPathName(compileResultPath, compileResultName);

	string exepath = Config::GetInstance()->GetValue("exepath");
	string exename = Config::GetInstance()->GetValue("exename");
	string exeResult = Config::GetInstance()->GetValue("exeresult");
	SetExePathNameResult(exepath, exename, exeResult);

}

string CompileWork::ResponseSendBuf(const string &rtMsg)
{
	string rtstr;
	responseHttp->SetMsgBody(rtMsg);
	rtstr = responseHttp->ResponseContent();

	cout << "\n\n" << rtstr << "\n\n";
	MyLogInstance->WriteLog("\n%s\n",rtstr.c_str());
	return rtstr;
}

void CompileWork::SetCppPathName(const string &cppPath, const string &cppName)
{
	this->cppPath = cppPath;
	this->cppName = cppName;
	if(this->cppName.empty())
		this->cppName = "compile.cpp";
	cppPathName = cppPath + "/" + this->cppName;
}

void CompileWork::SetCompileResultPathName(const string &filePath, const string &fileName)
{
	this->compileResultPath = filePath;
	this->compileResultName = fileName;
	if(this->compileResultName.empty())
		this->compileResultName = "compile.rs";
	compileResultPathName = compileResultPath + "/" + this->compileResultName;
}

void CompileWork::SetExePathNameResult(const string &filePath, const string &fileName, const string &resultFile)
{
	this->exePath = filePath;
	this->exeName = fileName;
	if(this->exeName.empty())
		this->exeName = "compile.exe";
	this->exeResult = resultFile;
	if(this->exeResult.empty())
		this->exeResult = "exe.rs";

	exePathName = exePath + "/" + exeName;
	exeResultPathName = exePath + "/" + exeResult;
}

int CompileWork::DetectionTextArea()
{
	string filter[] = {"system","System","thread","fork", "new","sleep",
								"Sleep","Window","window","while","for","malloc"};
	int num = sizeof(filter)/sizeof(string); // Maybe not safe
	for(int i = 0; i < num; i++)
	{
		int pos = textAreaValue.find(filter[i]);
		if(pos >= 0)
			return RT_ERR;
	}
	return RT_OK;
}

int CompileWork::GetTextAreaValue()
{
	string textareaname = Config::GetInstance()->GetValue("TEXTAREANAME");
	if(textareaname.empty())
		textareaname = TEXTAREANAME;
	textAreaValue = requestHttp->GetQueryParamValue(TEXTAREANAME);
	if(textAreaValue == RT_INFO[RT_ERR])
	{
		return RT_ERR;
	}
	textAreaValue = urldecode(textAreaValue);

	return RT_OK;
}
int CompileWork::WriteCPPToFile()
{
	fstream file(cppPathName.c_str(),ios::out);
	if(file.is_open()){
		file << textAreaValue.c_str();
	}
	else
	{
		//LOG("open file error");
		MyLogInstance->WriteLog("open file error");
		return RT_ERR_OPEN_FILE;
	}
	file.close();
	
	return MacToDos(cppPathName.c_str());
}
int CompileWork::CompileCPP()
{
	//string compileenv = Config::GetInstance()->GetValue("COMPILEEVN");
	//if(compileenv.empty())
	//	compileenv = COMPILEEVN;
	//system(COMPILEEVN.c_str());

	string compileexe = Config::GetInstance()->GetValue("COMPILEEXE");
	if(compileexe.empty())
		compileexe = COMPILEEXE;
	string cmp = compileexe + " -o " + exePathName + " " + cppPathName + " > " + compileResultPathName + " 2>&1";
	system(cmp.c_str());
	

	return RT_OK;
}
int CompileWork::ReadCompileResult()
{
	int error = 0;
	compileInfo="";
	char bf;
	char linebuf[255];
	fstream file(compileResultPathName.c_str(),ios::in);
	if(file.is_open()){
		while(file.getline(linebuf,255))
		{
			int pos = string(linebuf).find("error");
			if(pos>=0)
			{
				error = 1;
				compileInfo += string(linebuf,pos,-1) + "</br>";
			}
			else
			{
				pos = string(linebuf).find("undefined");
				if(pos >= 0)
				{
					error = 1;
					compileInfo += string(linebuf,pos,-1) + "</br>";
				}
			}
		}
	}
	else
	{
		//LOG("open file error");
		MyLogInstance->WriteLog("open file error");
		return RT_ERR_OPEN_FILE;
	}
	file.close();

	if( error )
	{
		cout << endl << "ERR:" << compileInfo << endl;
		return RT_ERR_COMPILE;
	}
	return RT_OK;
}
int CompileWork::ExecuteEXE()
{
	string exe = exePathName + " > " + exeResultPathName + " 2>&1";
	system(exe.c_str());

	return RT_OK;
}
int CompileWork::ReadExecuteResult()
{
	exeInfo = "";
	char bf;
	fstream file(exeResultPathName.c_str(),ios::in);
	if(file.is_open()){
		while(file.get(bf))
		{
			if(bf == '\n')
				exeInfo += "</br>";
			else
				exeInfo += bf;
		}
	}
	else
	{
		LOG("open file error");
		MyLogInstance->WriteLog("open file error");
		return RT_ERR_OPEN_FILE;
	}
	file.close();

	return RT_OK;
}

