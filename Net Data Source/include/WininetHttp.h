#pragma once
#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <vector>

using namespace std;

//每次读取的字节数
#define READ_BUFFER_SIZE        4096
#define USER_AGENT   "NERv/1.0"

enum HttpInterfaceError
{
	Hir_Success = 0,        //成功
	Hir_InitErr,            //初始化失败
	Hir_ConnectErr,            //连接HTTP服务器失败
	Hir_SendErr,            //发送请求失败
	Hir_QueryErr,            //查询HTTP请求头失败
	Hir_404,                //页面不存在
	Hir_IllegalUrl,            //无效的URL
	Hir_CreateFileErr,        //创建文件失败
	Hir_DownloadErr,        //下载失败
	Hir_QueryIPErr,            //获取域名对应的地址失败
	Hir_SocketErr,            //套接字错误
	Hir_UserCancel,            //用户取消下载
	Hir_BufferErr,            //文件太大，缓冲区不足
	Hir_HeaderErr,            //HTTP请求头错误
	Hir_ParamErr,            //参数错误，空指针，空字符
	Hir_UnknowErr,
};
enum HttpRequest
{
	Hr_Get,
	Hr_Post
};
class CWininetHttp
{
public:
	CWininetHttp(void);
	~CWininetHttp(void);

public:
	const std::string RequestInfo(const std::string& strUrl,
		HttpRequest type = Hr_Get,
		std::string lpHeader = "",
		std::string lpPostData = "",
		std::string lpUserAgent = USER_AGENT);

	const std::string ParseJsonInfo(const std::string& strUrl,
		HttpRequest type = Hr_Get,
		std::string lpHeader = "",
		std::string lpPostData = "",
		std::string lpUserAgent = USER_AGENT,
		std::vector<string> jsonURI = {"KEY"},
		int precision = 2,
		int divisor = 1);
protected:

	void Release();

	void ReleaseHandle(HINTERNET& hInternet);

	void ParseURLWeb(std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort,bool& isSSL);

	char* UtfToGbk(const char* utf8);

private:
	HINTERNET            m_hSession;
	HINTERNET            m_hConnect;
	HINTERNET            m_hRequest;
	HttpInterfaceError    m_error;
};
