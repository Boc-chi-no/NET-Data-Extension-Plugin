#pragma once
#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <vector>

using namespace std;

//ÿ�ζ�ȡ���ֽ���
#define READ_BUFFER_SIZE        4096
#define USER_AGENT   "NERv/1.0"

enum HttpInterfaceError
{
	Hir_Success = 0,        //�ɹ�
	Hir_InitErr,            //��ʼ��ʧ��
	Hir_ConnectErr,            //����HTTP������ʧ��
	Hir_SendErr,            //��������ʧ��
	Hir_QueryErr,            //��ѯHTTP����ͷʧ��
	Hir_404,                //ҳ�治����
	Hir_IllegalUrl,            //��Ч��URL
	Hir_CreateFileErr,        //�����ļ�ʧ��
	Hir_DownloadErr,        //����ʧ��
	Hir_QueryIPErr,            //��ȡ������Ӧ�ĵ�ַʧ��
	Hir_SocketErr,            //�׽��ִ���
	Hir_UserCancel,            //�û�ȡ������
	Hir_BufferErr,            //�ļ�̫�󣬻���������
	Hir_HeaderErr,            //HTTP����ͷ����
	Hir_ParamErr,            //�������󣬿�ָ�룬���ַ�
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
