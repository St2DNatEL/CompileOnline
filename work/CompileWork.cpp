#include "CompileWork.h"
#include "../Http/RequestHttp.h"
#include <fstream>

#define HTML "<!DOCTYPE html> <html> <head> \
	<title>���߱���</title> \
	<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>\
	 <!-- Bootstrap --> <link rel='stylesheet' href='http://cdn.bootcss.com/twitter-bootstrap/3.0.3/css/bootstrap.min.css'> \
	<script src='http://cdn.bootcss.com/jquery/1.10.2/jquery.min.js'></script> \
	<script src='http://cdn.bootcss.com/twitter-bootstrap/3.0.3/js/bootstrap.min.js'> \
	</script> <script src='/js/jquery.form.js'></script> <script> $(function() { \
	$('#form_code').ajaxForm({ dataType: 'text', //data: $('#form_code').serialize(), beforeSubmit: showRequest, success: showResponse }) }); \
	function showRequest(formData, jqForm, options) { } function showResponse(responseText, statusText) { alert('OK'); } </script> </head> \
	<body> <div class='container'> <p><h2>Ŀǰϵͳ��֧��c/c++���룬���ǻ�ƴ��Ŭ����ȫ���ϵͳ��^_^</h2></p> \
	<form id='form_code' name='form_code' class='form-horizontal' action='/test' method='post'> \
	<textarea id='text_code' name='text_code' class='form-control' rows='10'>%s</textarea> </br> \
	<input id='btn_code' type='submit' class='btn btn-primary' value='�� ��'></button> </form> \
	</br><div style='border:1px'><p><h3>���Ϊ��<h3></p>%s</div> \
	</div> </body> </html>"

/*#define HTML "<!DOCTYPE html> <html> <head> \
	<title>���߱���</title> \
	<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>\
	 <script> $(function() { \
	$('#form_code').ajaxForm({ dataType: 'text', //data: $('#form_code').serialize(), beforeSubmit: showRequest, success: showResponse }) }); \
	function showRequest(formData, jqForm, options) { } function showResponse(responseText, statusText) { alert('OK'); } </script> </head> \
	<body> <div class='container'> <p><h2>Ŀǰϵͳ��֧��c/c++���룬���ǻ�ƴ��Ŭ����ȫ���ϵͳ��^_^</h2></p> \
	<form id='form_code' name='form_code' class='form-horizontal' action='/test' method='post'> \
	<textarea id='text_code' name='text_code' class='form-control' rows='10'>%s</textarea> </br> \
	<input id='btn_code' type='submit' class='btn btn-primary' value='�� ��'></button> </form> \
	</br><div style='border:1px'><p><h3>���Ϊ��<h3></p>%s</div> \
	</div> </body> </html>"*/

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

int CompileWork::DoWork(const char *recvbuf, const int recvlen, string &sendbuf, int &sendlen)
{
	responseHttp->Init();
	requestHttp->Parse(recvbuf);

	if(GetTextAreaValue() != RT_OK)
	{
		sendbuf = ReturnSendBuf("");
		return RT_OK;
	}
	
	int rt = WriteCPPToFile();
	if(rt != RT_OK)
	{
		sendbuf = ReturnSendBuf("��������ʱ����Ϊ���ṩ����");
		return RT_OK;
	}

	rt = CompileCPP();
	if(rt != RT_OK)
	{
		sendbuf = ReturnSendBuf("��������ʱ����Ϊ���ṩ����");
		return RT_OK;
	}

	rt = ReadCompileResult();
	if(rt != RT_OK)
	{
		sendbuf = ReturnSendBuf("��������ʱ����Ϊ���ṩ����");
		return RT_OK;
	}

	rt = ExecuteEXE();
	if(rt != RT_OK)
	{
		sendbuf = ReturnSendBuf("��������ʱ����Ϊ���ṩ����");
		return RT_OK;
	}

	rt = ReadExecuteResult();
	if(rt != RT_OK)
	{
		sendbuf = ReturnSendBuf("��������ʱ����Ϊ���ṩ����");
		return RT_OK;
	}

	sendbuf = ReturnSendBuf(exeInfo);
	return RT_OK;
}

string CompileWork::ReturnSendBuf(const string &rtMsg)
{
	//todo: Restruct it 
	char rtchar[2048] = {0};
	string rtstr;
	
	string rtTXT = textAreaValue;
	rtTXT = ReplaceAll(rtTXT,"<","&lt;");
	rtTXT = ReplaceAll(rtTXT,">","&gt;");
	
	sprintf(rtchar, HTML, rtTXT.c_str(), rtMsg.c_str());
	responseHttp->SetMsgBody(string(rtchar));
	
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
	fstream file(compileResultPathName,ios::in);
	if(file.is_open()){
		file >> compileInfo;
	}
	else
	{
		//LOG("open file error");
		MyLogInstance->WriteLog("open file error");
		return RT_ERR_OPEN_FILE;
	}
	file.close();

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
	fstream file(exeResultPathName,ios::in);
	if(file.is_open()){
		file >> exeInfo;
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

