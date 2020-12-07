#include "WininetHttp.h"
#include "CJsonObject.hpp"
#include <fstream>
#include <tchar.h>
#include <vector>
#include <regex>
#include <defs.h>
#include <NERvGear/string.h>
#include <NERvGear/new.h>
#include <NERvGear/log.h>
#pragma comment(lib, "Wininet.lib")

using namespace NERvGear;
using namespace std;


// Name -:- {$1$} -:- Name
int JsonArrayIndex(string res) {
	string index_string;
	smatch result;
	regex expr("(^(\\{\\$)(\\d+)(\\$\\})$)");

	while (std::regex_search(res, result, expr)) {
		if (result.size() > 4) {
			index_string = result[3];
			break;
		}
	}

	if (!index_string.empty()) {
		int index = std::stoi(index_string);
		return index;
	}
	else {
		return -1;
	}
}

neb::CJsonObject position(neb::CJsonObject oJson, vector<string> res, int index=0) {
#ifdef _DEBUG
	std::string ojson_str;
	std::string res_str = res[index];
	wstring index_wt = to_wstring(index);

	if (index < res.size() - 1 && res.size() > 1) {
		int isJsonArray = JsonArrayIndex(res[index]);
		if (isJsonArray != -1) {
			ojson_str = oJson[isJsonArray].ToString();
			res_str = res[index];
			index_wt = to_wstring(index);
		}
		else {
			ojson_str = oJson[res[index]].ToString();
			res_str = res[index];
			index_wt = to_wstring(index);
		}

	}
	else if(res.size() > 1){
		int isJsonArray = JsonArrayIndex(res[index-1]);
		if (isJsonArray != -1) {
			ojson_str = oJson[isJsonArray].ToString();
			res_str = res[index-1];
			index_wt = to_wstring(index-1);
		}
		else {
			ojson_str = oJson[res[index - 1]].ToString();
			res_str = res[index - 1];
			index_wt = to_wstring(index - 1);
		}
	}
	if (!ojson_str.empty() && res.size() > 1) {
		NERvLogDebug(NVG_TEXT(NAME_STRING), NVG_TEXT("child position config[%s]: %s "), index_wt.c_str(), wstring(res_str.begin(), res_str.end()).c_str());
		NERvLogDebug(NVG_TEXT(NAME_STRING), NVG_TEXT("child position data: %s"), wstring(ojson_str.begin(), ojson_str.end()).c_str());
	}
	else if(res.size() > 1){
		NERvLogError(NVG_TEXT(NAME_STRING), NVG_TEXT("child position error config[%s]: %s"), index_wt.c_str(), wstring(res_str.begin(), res_str.end()).c_str());
		NERvLogError(NVG_TEXT(NAME_STRING), NVG_TEXT("child position error data: %s"), wstring(ojson_str.begin(), ojson_str.end()).c_str());
	}
#endif
	if (res.size() > 2 && index < res.size()-2) {	

		int isJsonArray = JsonArrayIndex(res[index]);
		if(isJsonArray!=-1)
			return position(oJson[isJsonArray],res,index+1);
		else
			return position(oJson[res[index]], res, index + 1);
	}
	else if (res.size() > 1 && index == res.size()-1) {

		int isJsonArray = JsonArrayIndex(res[index-1]);
		if (isJsonArray != -1)
			return position(oJson[isJsonArray], res, index + 1);
		else
			return oJson[res[index-1]];
	}

	int isJsonArray = JsonArrayIndex(res[index]);
	if (res.size() > 1) {
		if (isJsonArray != -1)
			return oJson[isJsonArray];
		else
			return oJson[res[index]];
	}else return oJson;

}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

CWininetHttp::CWininetHttp(void) :m_hSession(NULL), m_hConnect(NULL), m_hRequest(NULL)
{
}

CWininetHttp::~CWininetHttp(void)
{
	Release();
}

const std::string CWininetHttp::RequestInfo(const std::string& lpUrl,
	HttpRequest type/* = Hr_Get*/,
	std::string strHeader/*=""*/,
	std::string strUserAgent/*=""*/,
	std::string strPostData/*=""*/)
{
	std::string strRet = "";
	try
	{
		if (lpUrl.empty())
		{
			throw Hir_ParamErr;
		}
		Release();
		m_hSession = InternetOpen(s2ws(strUserAgent).c_str(), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);    //局部

		if (NULL == m_hSession)
		{
			throw Hir_InitErr;
		}

		DWORD rec_timeout = 15 * 10000;					// override the 15 second timeout
		InternetSetOption(m_hSession, INTERNET_OPTION_RECEIVE_TIMEOUT, &rec_timeout, sizeof(rec_timeout));


		INTERNET_PORT port = INTERNET_DEFAULT_HTTP_PORT;

		std::string strHostName, strPageName = "";
		bool is_ssl = false;

		ParseURLWeb(lpUrl, strHostName, strPageName, port, is_ssl);
#ifdef _DEBUG
		NERvLogInfo(NVG_TEXT(NAME_STRING), NVG_TEXT("RequestURL %s"), wstring(lpUrl.begin(), lpUrl.end()).c_str());
#endif
		DWORD dwFlags = (INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_RELOAD);
		if (is_ssl)
			dwFlags |= (INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID);

		m_hConnect = InternetConnectA(m_hSession, strHostName.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP, NULL, NULL);

		if (NULL == m_hConnect)
		{
			throw Hir_ConnectErr;
		}

		std::string strRequestType;
		if (Hr_Get == type)
		{
			strRequestType = "GET";
		}
		else
		{
			strRequestType = "POST";
		}

		m_hRequest = HttpOpenRequestA(m_hConnect, strRequestType.c_str(), strPageName.c_str(), "HTTP/1.1", NULL, NULL, dwFlags, NULL);
		if (NULL == m_hRequest)
		{
			throw Hir_InitErr;
		}

		DWORD dwHeaderSize = (strHeader.empty()) ? 0 : strlen(strHeader.c_str());
		BOOL bRet = FALSE;
		if (Hr_Get == type)
		{
			bRet = HttpSendRequestA(m_hRequest, strHeader.c_str(), dwHeaderSize, NULL, 0);
		}
		else
		{
			DWORD dwSize = (strPostData.empty()) ? 0 : strlen(strPostData.c_str());
			bRet = HttpSendRequestA(m_hRequest, strHeader.c_str(), dwHeaderSize, (LPVOID)strPostData.c_str(), dwSize);
		}
		if (!bRet)
		{
			throw Hir_SendErr;
		}

		char szBuffer[READ_BUFFER_SIZE + 1] = { 0 };
		DWORD dwReadSize = READ_BUFFER_SIZE;
		if (!HttpQueryInfoA(m_hRequest, HTTP_QUERY_RAW_HEADERS, szBuffer, &dwReadSize, NULL))
		{
			throw Hir_QueryErr;
		}
		if (NULL != strstr(szBuffer, "404"))
		{
			throw Hir_404;
		}

		while (true)
		{
			bRet = InternetReadFile(m_hRequest, szBuffer, READ_BUFFER_SIZE, &dwReadSize);
			if (!bRet || (0 == dwReadSize))
			{
				break;
			}
			szBuffer[dwReadSize] = '\0';
			strRet.append(UtfToGbk(szBuffer));
		}
	}
	catch (HttpInterfaceError error)
	{
		m_error = error;
	}
	return std::move(strRet);
}


const std::string CWininetHttp::ParseJsonInfo(const std::string& lpUrl,
	HttpRequest type/* = Hr_Get*/,
	std::string strHeader/*=""*/,
	std::string strPostData/*=""*/,
	std::string strUserAgent,
	std::vector<string> res/*=""*/,
	int precision,
	int divisor)
{
	string strJsonInfo = RequestInfo(lpUrl, type, strHeader, strUserAgent, strPostData);
	std::string iValue;
	neb::CJsonObject oJson;

	if (oJson.Parse(strJsonInfo))
	{
		int isJsonArray = JsonArrayIndex(res[res.size() - 1]);
		if (isJsonArray != -1)
			iValue = position(oJson, res).operator()(isJsonArray, to_string(precision), divisor);
		else
			iValue = position(oJson, res).operator()(res[res.size() - 1], to_string(precision), divisor);
		if (iValue.empty())
			return "定位JSON失败";
		else
			return iValue;
	}
	else
	{
		return "网站返回的不是JSON或者请求出错";
	}

}

void CWininetHttp::ParseURLWeb(std::string strUrl, std::string& strHostName, std::string& strPageName, WORD& sPort,bool& isSSL)
{
	sPort = 80;
	string strTemp(strUrl);
	std::size_t nPos = strTemp.find("http://");
	if (nPos != std::string::npos)
	{
		strTemp = strTemp.substr(nPos + 7, strTemp.size() - nPos - 7);
		isSSL = false;
	}
	else
	{
		nPos = strTemp.find("https://");
		if (nPos != std::string::npos)
		{
			sPort = 443;//INTERNET_DEFAULT_HTTPS_PORT;
			isSSL = true;
			strTemp = strTemp.substr(nPos + 8, strTemp.size() - nPos - 8);
		}
	}


	nPos = strTemp.find('/');
	if (nPos == std::string::npos)
		strHostName = strTemp;
	else
		strHostName = strTemp.substr(0, nPos);

	std::size_t nPos1 = strHostName.find(':');
	if (nPos1 != std::string::npos)
	{
		std::string strPort = strTemp.substr(nPos1 + 1, strHostName.size() - nPos1 - 1);
		strHostName = strHostName.substr(0, nPos1);
		sPort = (WORD)atoi(strPort.c_str());
	}
	if (nPos == std::string::npos)
	{
		strPageName = '/';
		return;
	}
	strPageName = strTemp.substr(nPos, strTemp.size() - nPos);
}

void CWininetHttp::Release()
{
	ReleaseHandle(m_hRequest);
	ReleaseHandle(m_hConnect);
	ReleaseHandle(m_hSession);
}

void CWininetHttp::ReleaseHandle(HINTERNET& hInternet)
{
	if (hInternet)
	{
		InternetCloseHandle(hInternet);
		hInternet = NULL;
	}
}

char* CWininetHttp::UtfToGbk(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	if (len <= 0)
		return (char*)utf8;
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	if (len <= 0)
		return (char*)utf8;
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}
