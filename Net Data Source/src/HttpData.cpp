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

//GUIDʹ��guidgen����
const UID HttpData::ID = { 0x2f94132f, 0x4489, 0x4375, {  0x6e, 0xfd, 0x27, 0xbf, 0x50, 0x3, 0xc8, 0x55 } }; // {}

//������λ
//static const wstring UNIT_NAME[] = { L"B", L"KB", L"MB", L"GB" };

//������λ��'\0'�ָ������'\0'��Ϊ���ַ�
//ע�⣺��ĳ��λ�����ֿ�ͷ����"1KB",�˴����ַ�'1'������ǰһλ�ָ���'\0'��ϳɰ˽���ת���ַ�����'\0'дΪ'\000'�ɱ���
//		������	"1B\01KB\0"		��	'1'+'B'+'\01'+'K'+'B'+'\0'
//			��	"1B\0001KB\0"	��	'1'+'B'+'\000'+'1'+'K'+'B'+'\0'
//static const wchar_t  UNIT_ENUM[] = L"B\0KB\0MB\0GB\0";

/// ============================================================================
/// implementation
/// ============================================================================

std::wstring StringToWstring(const std::string str)
{// stringתwstring
	unsigned len = str.size() * 2;// Ԥ���ֽ���
	setlocale(LC_CTYPE, "");     //������ô˺���
	wchar_t* p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
	mbstowcs(p, str.c_str(), len);// ת��
	std::wstring str1(p);
	delete[] p;// �ͷ�������ڴ�
	return str1;
}

std::string WstringToString(const std::wstring str)
{// wstringתstring
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

//�����޸�
long HttpData::GetId(UID* id)
{
    if (id == NULL)
        return E_INVALIDARG;

    *id = ID;

    return S_OK;
}

//Data���ƣ���ʾ��Data Picker���ĵڶ���
size_t HttpData::GetName(size_t len, wchar_t* name)
{
    return NERvTranslation(_TRANSLATE("HTTP����Դ����"), len, name);
}

//Data��������ʾ��Data Picker�����·��ı���
size_t HttpData::GetDescrip(size_t len, wchar_t* descrip)
{
    return NERvTranslation(_TRANSLATE("����HTTP�ӿ�,����Json,Html,Text��ʽ����"), len, descrip);
}

//��ȡ��Data�ĸ�Data Source
//����ִ�н����һ�㲻���޸�
long HttpData::QuerySource(IDataSource** source)
{
    if (source == NULL)
        return E_INVALIDARG;

    *source = m_source;
    (*source)->AddRef();

    return S_OK;
}

//��ȡ��Data����Value����
//�����޷������Σ�һ�㲻���޸�
unsigned HttpData::GetValueCount()
{
    return VALUE_COUNT;
}

//��ȡValue
//index	ѡ��Value�����
//nbyte	��������ΪDATA::STRINGʱ������buf����
//buf	���뻺�������Ը��ڴ渳ֵ��ע�⸳ֵ����
//���ػ���������
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

//��ȡValue���ֵ
//index	ѡ��Value�����
//nbyte	��������ΪDATA::STRINGʱ������buf����
//buf	���뻺�������Ը��ڴ渳ֵ��ע�⸳ֵ����
//���ػ���������
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

//��ȡValue��Сֵ
//index	ѡ��Value�����
//nbyte	��������ΪDATA::STRINGʱ������buf����
//buf	���뻺�������Ը��ڴ渳ֵ��ע�⸳ֵ����
//���ػ���������
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

//��ȡValue����
//index	ѡ��Value�����
//���ض�ӦValue��������
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

//��ȡValue������λ
//index	ѡ��Value�����
//len	����unit���峤��
//unit	�����ַ������������Ը�����д�������λ�ַ���
//���ػ���������
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

//��ȡValue����
//index	ѡ��Value�����
//len	����name���峤��
//name	�����ַ������������Ը�����д��Value�����ַ���
//���ػ���������
size_t HttpData::GetValueName(unsigned index, size_t len, wchar_t* name)
{
    switch (index) {
	case STR_JSON_VALUE: return NERvTranslation(_TRANSLATE("Json"), len, name);
	case STR_TEXT_VALUE: return NERvTranslation(_TRANSLATE("Html"), len, name);
    default: break;
    }

    return 0;
}

//��ȡValue����
//index	ѡ��Value�����
//len	����descrip���峤��
//descrip	�����ַ������������Ը�����д��Value�����ַ���
//���ػ���������
size_t HttpData::GetValueDescrip(unsigned index, size_t len, wchar_t* descrip)
{
    switch (index) {
	case STR_JSON_VALUE: return NERvTranslation(_TRANSLATE("Example: \n{\r\n    \"method\":\"POST/GET\",\r\n    \"request_url\":\"����URL\",\r\n    \"json_uri\":\"json��λ\",\r\n    \"header\":\"����ͷ\",\r\n    \"post_body\":\"post����\",\r\n    \"user_agent\":\"UAͷ\",\r\n    \"precision\":2, //С���㾫��\r\n    \"divisor\":1000 //����\r\n}"), len, descrip);
	case STR_TEXT_VALUE: return NERvTranslation(_TRANSLATE("Example: \n{\r\n    \"request_url\":\"����URL\"   }"), len, descrip);
	//case U32_DATA_VALUE_2: // value 2 has no description
    default: break;
    }

    return 0;
}

//��ȡValue���¼��
//index	ѡ��Value�����
//���ظ��¼�����޷�������
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

//��������ʱ����
//���ݴ�����GetValue
//index	ѡ��Value�����
//param	����Update Parameter
//����ִ�н��
//m_cur1
long HttpData::Update(unsigned index, const wchar_t* param)
{
	CWininetHttp whttp = CWininetHttp();
    if (index >= VALUE_COUNT)
        return E_INVALIDARG;

	//static const unsigned CUR_VALUE_B = unsigned(1) * 1024 * 1024 * 1024; // 1 GB
	//static const unsigned MAX_VALUE_B = unsigned(3) * 1024 * 1024 * 1024; // 3 GB

    const bool SINGLE = (index != unsigned(NVG_ANY));

	//TODO:�첽֧��,�߳���
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
			m_cur1 = L"���������";
		else {
			string strc = WstringToString(m_config);

			if (Config.Parse(strc)){
				if (Config.Get("method", method)) {
					if (Config.Get("request_url", requestURI)) {
						if (Config.Get("json_uri", jsonURI)) {
						
							std::vector<string> res = split(jsonURI, delim);

							if (res.size() < 1) {
								m_cur1 = L"��λURI����";
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
								m_cur1 = L"���󷽷�ֻ��GET/POST";
								return S_OK;
							}

							string json = whttp.ParseJsonInfo(requestURI, hr_method, header, postBody, userAgent, res, precision, divisor);
							m_cur1 = StringToWstring(json);
						}else m_cur1 = L"��������Json��λ";
					}else m_cur1 = L"������������URL";
				}else m_cur1 = L"�����������󷽷�";
			}else m_cur1 =  L"����Json��������";
		}
        if (SINGLE) break;
	case STR_TEXT_VALUE:
		m_config = param;
		if (m_config.empty())
			m_cur1 = L"���������";
		else {
			string strc = WstringToString(m_config);

			if (Config.Parse(strc))
			{
				if (Config.Get("request_url", requestURI)) {
					string text = whttp.RequestInfo(requestURI, Hr_Get, "", "");
					m_cur1 = StringToWstring(text);
				}else m_cur1 = L"������������URL";
			}else m_cur1 = L"����Json��������";
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

//Ԥ��
long HttpData::Invoke(unsigned index, const wchar_t* param, UI::IWindow* window)
{
    return S_FALSE;
}

//���ã������޸Ľ�����Parameter
//��ʼ��������Config��ť���ᴥ��
//index	ѡ��Value�����
//type	ָ����ǰ����Parameter����[Update/Invoke��]
//window	ָ�����壬Ԥ��
//len	����param���峤��
//param	�����ַ�������������Ϊ��NULLֵ���޸Ķ�ӦParameter
//���ػ��������ȣ���Ϊ0�����Config��ť
size_t HttpData::Config(unsigned index, DATA::CONFIG type, UI::IWindow* window, size_t len, wchar_t* param)
{
	return 0;
}

//����Value������λ
//ѡ�������λʱ����
//index	ѡ��Value�����
//unit	����ָ��������λ�ַ���
//����ִ�н��
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

//ö��Value������λ
//index	ѡ��Value�����
//len	����unit���峤��
//units	�����ַ������������Ը�����д�������λ�ַ���
//���ػ���������
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
