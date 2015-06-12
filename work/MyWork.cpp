#include "MyWork.h"

string compile="<!DOCTYPE html> <html> <head> <title>在线编译</title> <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'> <!-- Bootstrap --> <link rel='stylesheet' href='http://cdn.bootcss.com/twitter-bootstrap/3.0.3/css/bootstrap.min.css'> <script src='http://cdn.bootcss.com/jquery/1.10.2/jquery.min.js'></script> <script src='http://cdn.bootcss.com/twitter-bootstrap/3.0.3/js/bootstrap.min.js'></script> <script src='/js/jquery.form.js'></script> <script> $(function() { $('#form_code').ajaxForm({ dataType: 'text', //data: $('#form_code').serialize(), beforeSubmit: showRequest, success: showResponse }) }); function showRequest(formData, jqForm, options) { } function showResponse(responseText, statusText) { alert('OK'); } </script> </head> <body> <div class='container'> <p><h2>目前系统仅支持c/c++代码，我们会拼命努力健全这个系统的^_^</h2></p> <form id='form_code' name='form_code' class='form-horizontal' action='/test' method='post'> <textarea id='text_code' name='text_code' class='form-control' rows='20'></textarea> </br><input id=\"input_img\" name=\"input_img\" type=\"file\" size=\"10\"></br> <input id='btn_code' type='submit' class='btn btn-primary' value='提 交'></button> </form> </div> </body> </html>";
MyWork::MyWork(void)
{
}

MyWork::~MyWork(void)
{
}


int MyWork::DoWork(const char * recvbuf,const int recvlen,string &sendbuf,int & sendlen)
{
	sendbuf = string("<html><head></head></br><body>");

	Http myhttp;
	myhttp.Parse((char*)recvbuf);
	sendbuf += compile;
	LOG(myhttp.GetQueryParamValue("text_code").c_str(), __FILE__, __LINE__);
	sendbuf += string("</body></html>");
	sendlen = strlen(sendbuf.c_str());
	return RT_OK;
}