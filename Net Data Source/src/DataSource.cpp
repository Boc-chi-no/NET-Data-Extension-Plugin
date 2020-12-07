#include "DataSource.h"
#include "HttpData.h"
#include "defs.h"

#include <NERvGear/string.h>
#include <NERvGear/new.h>
#include <NERvGear/log.h>
#include <NERvGear/component/CDataSource.h>

using namespace NERvGear;

#ifdef _TRANSLATE
#undef _TRANSLATE
#endif

#define _TRANSLATE(_STR) L##_STR

/// ----------------------------------------------------------------------------
/// variables
/// ----------------------------------------------------------------------------

/// ----------------------------------------------------------------------------
/// object declaration
/// ----------------------------------------------------------------------------

NVG_BEGIN_OBJECT_INFO(DataSource)
    NVG_DECLARE_OBJECT_CLASSID_UID(ID_CDataSource)
	//GUIDʹ��guidgen����
	NVG_DECLARE_OBJECT_OBJECTID(0x2f94132f, 0x4489, 0x4375, 0x55, 0x0, 0x35, 0x70, 0x29, 0xb2, 0x1d, 0x90) // {}
    NVG_DECLARE_OBJECT_VERSION(1, 0, 2)
	//Data Source���ƣ�ע�����ƣ�����ʾ��Data Picker�е�����
    NVG_DECLARE_OBJECT_NAME("NET Data Source Extension")
	//Data Source������ͬ�ϣ�����ʾ��Data Picker�е�����
    NVG_DECLARE_OBJECT_DESCRIP("Data Data Source Component")
NVG_END_OBJECT_INFO()

/// ----------------------------------------------------------------------------
/// interface information
/// ----------------------------------------------------------------------------

NVG_BEGIN_INTERFACE_INFO(DataSource)
    NVG_DECLARE_INTERFACE(ID_IUnknown, IUnknown, this)
    NVG_DECLARE_INTERFACE(ID_IDataSource, IDataSource, this)
NVG_END_INTERFACE_INFO()

/// ============================================================================
/// implementation
/// ============================================================================

//�����޸�
long DataSource::GetId(NERvGear::UID* id)
{
    if (id == NULL)
        return E_INVALIDARG;

    *id = STATIC_OBJECT_INFO.objectID;

    return S_OK;
}

//Data Source���ƣ���ʾ��Data Picker���ĵ�һ��
size_t DataSource::GetName(size_t len, wchar_t* name)
{
	return NERvTranslation(_TRANSLATE("�������ݽ���"), len, name);
}

//Data Source��������ʾ��Data Picker�����·��ı���
size_t DataSource::GetDescrip(size_t len, wchar_t* descrip)
{
	return NERvTranslation(_TRANSLATE("�����������ϵ�����"), len, descrip);
}

//�����޸�
unsigned DataSource::GetDataCount()
{
    return DATA_COUNT;
}

//��ȡData����
//index	ѡ��Data�����
//data	��ò���д��Data����
//����ִ�н��
long DataSource::GetData(unsigned index, NERvGear::IData** data)
{
    if (index >= DATA_COUNT || data == NULL)
        return E_INVALIDARG;

	switch (index) {
	case DATA_DATA_1: *data = nvg_new HttpData(this); break;
	//case DATA_DATA_2: *data = nvg_new DataData2(this); break;
	default: return E_FAIL;
	}

    return S_OK;
}

//ͨ��ID��ȡData����
//id	�����ID
//data	��ò���д��Data����
//����ִ�н��
long DataSource::FindData(const NERvGear::UID& id, NERvGear::IData** data)
{
    if (data == NULL)
        return E_INVALIDARG;

    if (id == HttpData::ID) {
		*data = nvg_new HttpData(this);
	} /*else if (id == DataData2::ID) {
		*data = nvg_new DataData2(this);
	}*/ else { return E_FAIL; }

    return S_OK;
}

/// ----------------------------------------------------------------------------
/// constructor & destructor
/// ----------------------------------------------------------------------------

DataSource::DataSource()
{
#ifdef _DEBUG
	NERvLogInfo(NAME_STRING, L"DataSource::DataSource()");
#endif
}

DataSource::~DataSource()
{
#ifdef _DEBUG
	NERvLogInfo(NAME_STRING, L"DataSource::~DataSource()");
#endif
}

/// ----------------------------------------------------------------------------
/// utilities functions
/// ----------------------------------------------------------------------------

/// ----------------------------------------------------------------------------
/// event handlers
/// ----------------------------------------------------------------------------


