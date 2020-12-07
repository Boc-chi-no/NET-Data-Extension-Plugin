#include "HttpData.h"
#include "DataSource.h"
#include "defs.h"

#include <NERvGear/string.h>

#ifdef _TRANSLATE
#undef _TRANSLATE
#endif
#include <time.h>
#include <WininetHttp.h>
#include <CJsonObject.hpp>
#include <clocale>

#define _TRANSLATE(_STR) L##_STR

using namespace NERvGear;
using namespace std;

/// ----------------------------------------------------------------------------
/// variables
/// ----------------------------------------------------------------------------

//GUID使用guidgen生成
const UID HttpData::ID = { 0x2f94132f, 0x4489, 0x4375, {  0x6e, 0xfd, 0x27, 0xbf, 0x50, 0x3, 0xc8, 0x55 } }; // {}

//计量单位
//static const wstring UNIT_NAME[] = { L"B", L"KB", L"MB", L"GB" };

//计量单位以'\0'分割，不可以'\0'作为首字符
//注意：若某单位以数字开头，如"1KB",此处的字符'1'可能与前一位分隔符'\0'组合成八进制转义字符，将'\0'写为'\000'可避免
//		例：×	"1B\01KB\0"		→	'1'+'B'+'\01'+'K'+'B'+'\0'
//			√	"1B\0001KB\0"	→	'1'+'B'+'\000'+'1'+'K'+'B'+'\0'
//static const wchar_t  UNIT_ENUM[] = L"B\0KB\0MB\0GB\0";

/// ============================================================================
/// implementation
/// ============================================================================

std::wstring StringToWstring(const std::string str)
{// string转wstring
	unsigned len = str.size() * 2;// 预留字节数
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t* p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs(p, str.c_str(), len);// 转换
	std::wstring str1(p);
	delete[] p;// 释放申请的内存
	return str1;
}

std::string WstringToString(const std::wstring str)
{// wstring转string
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char* p = new char[len];
	wcstombs(p, str.c_str(), len);
	std::string str1(p);
	delete[] p;
	return str1;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	char* strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char* p = strtok(strs, d);
	while (p) {
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);
	}

	return res;
}

//不需修改
long HttpData::GetId(UID* id)
{
    if (id == NULL)
        return E_INVALIDARG;

    *id = ID;

    return S_OK;
}

//Data名称，显示在Data Picker树的第二层
size_t HttpData::GetName(size_t len, wchar_t* name)
{
    return NERvTranslation(_TRANSLATE("HTTP数据源解析"), len, name);
}

//Data描述，显示在Data Picker的右下方文本框
size_t HttpData::GetDescrip(size_t len, wchar_t* descrip)
{
    return NERvTranslation(_TRANSLATE("调用HTTP接口,解析Json,Html,Text格式数据"), len, descrip);
}

//获取该Data的父Data Source
//返回执行结果，一般不需修改
long HttpData::QuerySource(IDataSource** source)
{
    if (source == NULL)
        return E_INVALIDARG;

    *source = m_source;
    (*source)->AddRef();

    return S_OK;
}

//获取该Data的子Value个数
//返回无符号整形，一般不需修改
unsigned HttpData::GetValueCount()
{
    return VALUE_COUNT;
}

//获取Value
//index	选中Value的序号
//nbyte	数据类型为DATA::STRING时，传入buf长度
//buf	传入缓冲区，对该内存赋值，注意赋值类型
//返回缓冲区长度
size_t HttpData::GetValue(unsigned index, size_t nbyte, void* buf)
{
    switch (index) {
	case STR_JSON_VALUE: return NERvCopyString(m_cur1.c_str(), nbyte / 2, static_cast<wchar_t*>(buf)) * 2;
	case STR_TEXT_VALUE: return NERvCopyString(m_cur1.c_str(), nbyte / 2, static_cast<wchar_t*>(buf)) * 2;
	//case U32_DATA_VALUE_2:
	//	if (buf)
	//		*reinterpret_cast<unsigned*>(buf) = m_cur2;

	//	return sizeof(unsigned);
    default: break;
    }

    return 0;
}

//获取Value最大值
//index	选中Value的序号
//nbyte	数据类型为DATA::STRING时，传入buf长度
//buf	传入缓冲区，对该内存赋值，注意赋值类型
//返回缓冲区长度
size_t HttpData::GetMaximum(unsigned index, size_t nbyte, void* buf)
{
	switch (index) {
	case STR_JSON_VALUE: break;	// value 1 has no maximum value
	case STR_TEXT_VALUE: break;	// value 1 has no maximum value

	//case U32_DATA_VALUE_2:
	//	if (buf) 
	//		*reinterpret_cast<unsigned*>(buf) = m_max2;

	//	return sizeof(unsigned);
	default: break;
	}

    return 0;
}

//获取Value最小值
//index	选中Value的序号
//nbyte	数据类型为DATA::STRING时，传入buf长度
//buf	传入缓冲区，对该内存赋值，注意赋值类型
//返回缓冲区长度
size_t HttpData::GetMinimum(unsigned index, size_t nbyte, void* buf)
{
	switch (index) {
	case STR_JSON_VALUE: break;	// value 1 has no minimum value
	case STR_TEXT_VALUE: break;	// value 1 has no minimum value
	/*case U32_DATA_VALUE_2:
		if (buf)
			*reinterpret_cast<unsigned*>(buf) = 0;

		return sizeof(unsigned);*/
	default: break;
	}

    return 0;
}

unsigned HttpData::GetValueIndex(int id)
{
	if (id > NVG_ANY && id < VALUE_COUNT)
		return id;

	return NVG_ANY;
}

int HttpData::GetValueId(unsigned index)
{
	if (index < VALUE_COUNT)
		return index;

	return NVG_ANY;
}

//获取Value类型
//index	选中Value的序号
//返回对应Value数据类型
DATA::TYPE HttpData::GetValueType(unsigned index)
{
    switch (index) {
	case STR_JSON_VALUE: return DATA::STRING;
	case STR_TEXT_VALUE: return DATA::STRING;
	//case STR_TEXT_VALUE: return DATA::UNSIGNED;
    default: break;
    }

    return DATA::INVALID;
}

//获取Value计量单位
//index	选中Value的序号
//len	传入unit缓冲长度
//unit	传入字符串缓冲区，对该区域写入计量单位字符串
//返回缓冲区长度
size_t HttpData::GetValueUnit(unsigned index, size_t len, wchar_t* unit)
{
	switch (index) {
	case STR_JSON_VALUE: break;	// value 1 has no unit
	case STR_TEXT_VALUE: break;	// value 1 has no unit
	//case U32_DATA_VALUE_2: return NERvCopyString(UNIT_NAME[m_unit2].c_str(), len, unit);	// do not translate units
	default: break;
	}
	

    return 0;
}

//获取Value名称
//index	选中Value的序号
//len	传入name缓冲长度
//name	传入字符串缓冲区，对该区域写入Value名称字符串
//返回缓冲区长度
size_t HttpData::GetValueName(unsigned index, size_t len, wchar_t* name)
{
    switch (index) {
	case STR_JSON_VALUE: return NERvTranslation(_TRANSLATE("Json"), len, name);
	case STR_TEXT_VALUE: return NERvTranslation(_TRANSLATE("Html"), len, name);
    default: break;
    }

    return 0;
}

//获取Value描述
//index	选中Value的序号
//len	传入descrip缓冲长度
//descrip	传入字符串缓冲区，对该区域写入Value描述字符串
//返回缓冲区长度
size_t HttpData::GetValueDescrip(unsigned index, size_t len, wchar_t* descrip)
{
    switch (index) {
	case STR_JSON_VALUE: return NERvTranslation(_TRANSLATE("Example: \n{\r\n    \"method\":\"POST/GET\",\r\n    \"request_url\":\"请求URL\",\r\n    \"json_uri\":\"json定位\",\r\n    \"header\":\"请求头\",\r\n    \"post_body\":\"post数据\",\r\n    \"user_agent\":\"UA头\",\r\n    \"precision\":2, //小数点精度\r\n    \"divisor\":1000 //除数\r\n}"), len, descrip);
	case STR_TEXT_VALUE: return NERvTranslation(_TRANSLATE("Example: \n{\r\n    \"request_url\":\"请求URL\"   }"), len, descrip);
	//case U32_DATA_VALUE_2: // value 2 has no description
    default: break;
    }

    return 0;
}

//获取Value更新间隔
//index	选中Value的序号
//返回更新间隔，无符号整形
unsigned HttpData::GetUpdateInterval(unsigned index)
{
    switch (index) {
	case STR_JSON_VALUE: return 60000;    // 60 sencod
	case STR_TEXT_VALUE: return 60000;    // 60 sencod
	//case U32_DATA_VALUE_2: return 1000;	// 1 sencod
    default: break;
    }

    return NVG_ANY;
}

//更新数据时调用
//数据传出在GetValue
//index	选中Value的序号
//param	传入Update Parameter
//返回执行结果
//m_cur1
long HttpData::Update(unsigned index, const wchar_t* param)
{
	CWininetHttp whttp = CWininetHttp();
    if (index >= VALUE_COUNT)
        return E_INVALIDARG;

	//static const unsigned CUR_VALUE_B = unsigned(1) * 1024 * 1024 * 1024; // 1 GB
	//static const unsigned MAX_VALUE_B = unsigned(3) * 1024 * 1024 * 1024; // 3 GB

    const bool SINGLE = (index != unsigned(NVG_ANY));

	//TODO:异步支持,线程锁
	const string delim = "-:-";

	neb::CJsonObject Config;
	std::string requestURI;
	std::string userAgent;
	std::string postBody;
	std::string jsonURI;
	std::string method;
	std::string header;
	int precision;
	int divisor;

    switch (index) {
    case NVG_ANY:   // update all
    case STR_JSON_VALUE:
		m_config = param;
		if (m_config.empty())
			m_cur1 = L"请输入参数";
		else {
			string strc = WstringToString(m_config);

			if (Config.Parse(strc)){
				if (Config.Get("method", method)) {
					if (Config.Get("request_url", requestURI)) {
						if (Config.Get("json_uri", jsonURI)) {
						
							std::vector<string> res = split(jsonURI, delim);

							if (res.size() < 1) {
								m_cur1 = L"定位URI有误";
								return S_OK;
							}
							HttpRequest hr_method;
							if (!Config.Get("precision", precision))
								precision = 2;
							if (!Config.Get("divisor", divisor))
								divisor = 1;
							if (!Config.Get("user_agent", userAgent))
								userAgent = USER_AGENT;
							Config.Get("header", header);


							if (method == "GET") {
								hr_method = Hr_Get;
							}
							else if (method == "POST") {
								hr_method = Hr_Post;
								Config.Get("post_body", postBody);
							}
							else
							{
								m_cur1 = L"请求方法只能GET/POST";
								return S_OK;
							}

							string json = whttp.ParseJsonInfo(requestURI, hr_method, header, postBody, userAgent, res, precision, divisor);
							m_cur1 = StringToWstring(json);
						}else m_cur1 = L"必须输入Json定位";
					}else m_cur1 = L"必须输入请求URL";
				}else m_cur1 = L"必须输入请求方法";
			}else m_cur1 =  L"输入Json参数错误";
		}
        if (SINGLE) break;
	case STR_TEXT_VALUE:
		m_config = param;
		if (m_config.empty())
			m_cur1 = L"请输入参数";
		else {
			string strc = WstringToString(m_config);

			if (Config.Parse(strc))
			{
				if (Config.Get("request_url", requestURI)) {
					string text = whttp.RequestInfo(requestURI, Hr_Get, "", "");
					m_cur1 = StringToWstring(text);
				}else m_cur1 = L"必须输入请求URL";
			}else m_cur1 = L"输入Json参数错误";
		}
		if (SINGLE) break;
	//case U32_DATA_VALUE_2:
	//	m_cur2 = CUR_VALUE_B;
	//	m_max2 = MAX_VALUE_B;

	//	switch (m_unit2) {
	//	case UNIT_GB: m_cur2 /= 1024; m_max2 /= 1024;
	//	case UNIT_MB: m_cur2 /= 1024; m_max2 /= 1024;
	//	case UNIT_KB: m_cur2 /= 1024; m_max2 /= 1024;
	//	case UNIT_B:
	//	default: break;
	//	}

	//	if (SINGLE) break;
	default: break;
    }

    return S_OK;
}

//预留
long HttpData::Invoke(unsigned index, const wchar_t* param, UI::IWindow* window)
{
    return S_FALSE;
}

//配置，用于修改界面上Parameter
//初始化、单击Config按钮均会触发
//index	选中Value的序号
//type	指定当前配置Parameter类型[Update/Invoke等]
//window	指定窗体，预留
//len	传入param缓冲长度
//param	传入字符串缓冲区，若为非NULL值可修改对应Parameter
//返回缓冲区长度，若为0则禁用Config按钮
size_t HttpData::Config(unsigned index, DATA::CONFIG type, UI::IWindow* window, size_t len, wchar_t* param)
{
	return 0;
}

//设置Value计量单位
//选择计量单位时触发
//index	选中Value的序号
//unit	传入指定计量单位字符串
//返回执行结果
long HttpData::SetValueUnit(unsigned index, const wchar_t* unit)
{
	if (index >= VALUE_COUNT)
		return E_INVALIDARG;

	switch (index) {
	case STR_JSON_VALUE: break; // value 1 has no unit
	case STR_TEXT_VALUE: break; // value 1 has no unit
	//case U32_DATA_VALUE_2:
	//{
	//	const wstring str(unit);

	//	for (unsigned i = 0; i < UNIT_COUNT; ++i) {
	//		if (str == UNIT_NAME[i]) {
	//			m_unit2 = static_cast<UNIT_TYPE>(i);
	//			return S_OK;
	//		}
	//	}
	//}
	default: break;
	}

    return E_FAIL;
}

//枚举Value计量单位
//index	选中Value的序号
//len	传入unit缓冲长度
//units	传入字符串缓冲区，对该区域写入计量单位字符串
//返回缓冲区长度
size_t HttpData::EnumValueUnit(unsigned index, size_t len, wchar_t* units)
{
	switch (index) {
	case STR_JSON_VALUE: break; // value 1 has no unit
	case STR_TEXT_VALUE: break; // value 1 has no unit
	//case U32_DATA_VALUE_2: return NERvCopyStringDoubleNull(UNIT_ENUM, len, units);
	default: break;
	}

	return 0;
}

//??
long HttpData::SetEventListener(IEventHandler* listener)
{
    return E_NOTIMPL;
}


/// ----------------------------------------------------------------------------
/// constructor & destructor
/// ----------------------------------------------------------------------------

HttpData::HttpData(DataSource* source) : m_source(source)//, m_unit2(UNIT_B)
{
	if (m_source)
		m_source->AddRef();
}

HttpData::~HttpData()
{
	if (m_source) {	// optional: use NERvRelease() instead
		m_source->Release();
		m_source = NULL;
	}
}

/// ----------------------------------------------------------------------------
/// utilities functions
/// ----------------------------------------------------------------------------

/// ----------------------------------------------------------------------------
/// event handlers
/// ----------------------------------------------------------------------------
