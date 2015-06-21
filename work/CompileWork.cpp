#include "CompileWork.h"
#include "../Http/RequestHttp.h"
#include <fstream>

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
	char buf[255] = {0};
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
	string dir = COMPILEWOKRDIR + path;
	fstream tmpFile(dir,ios::in | ios::_Nocreate);
	if(tmpFile.is_open())
	{
		while(tmpFile.get(bf))
		{
			respMsg += bf;
		}
	}
	else
	{
		responseHttp->SetStatus("404 Not Found");
		respMsg = "";
	}
	tmpFile.close();

	responseHttp->SetMsgBody(respMsg);
	respMsg = responseHttp->ResponseContent();

	return RT_OK;
	
}

int CompileWork::DoCompile(string &sendbuf)
{
	if(GetTextAreaValue() != RT_OK)
	{
		sendbuf = ResponseSendBuf("");
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
		return RT_OK;
	}

	
	ResponseStaticPage(sendbuf);
	return RT_OK;
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
	cppPathName = cppPath + "\\" + cppName;
}

void CompileWork::SetCompileResultPathName(const string &filePath, const string &fileName)
{
	this->compileResultPath = filePath;
	this->compileResultName = fileName;
	compileResultPathName = compileResultPath + "\\" + compileResultName;
}

void CompileWork::SetExePathNameResult(const string &filePath, const string &fileName, const string &resultFile)
{
	this->exePath = filePath;
	this->exeName = fileName;
	this->exeResult = resultFile;

	exePathName = exePath + "\\" + exeName;
	exeResultPathName = exePath + "\\" + exeResult;
}

int CompileWork::GetTextAreaValue()
{
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
	fstream file(cppPathName,ios::out);
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
	system(COMPILEEVN.c_str());

	string cmp = COMPILEEXE + " -o " + exePathName + " " + cppPathName + " > " + compileResultPathName;
	system(cmp.c_str());
	

	return RT_OK;
}
int CompileWork::ReadCompileResult()
{
	compileInfo="";
	char bf;
	fstream file(compileResultPathName,ios::in);
	if(file.is_open()){
		while(file.get(bf))
			compileInfo += bf;
	}
	else
	{
		//LOG("open file error");
		MyLogInstance->WriteLog("open file error");
		return RT_ERR_OPEN_FILE;
	}
	file.close();

	int ps = compileInfo.find("err");
	if( ps >=0)
	{
		cout << endl << "ERR:" << compileInfo << endl;
		return RT_ERR_COMPILE;
	}
	return RT_OK;
}
int CompileWork::ExecuteEXE()
{
	string exe = exePathName + " > " + exeResultPathName;
	system(exe.c_str());

	return RT_OK;
}
int CompileWork::ReadExecuteResult()
{
	exeInfo = "";
	char bf;
	fstream file(exeResultPathName,ios::in);
	if(file.is_open()){
		while(file.get(bf))
			exeInfo += bf;
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

