
#ifndef HttpData_H
#define HttpData_H

class DataSource;

#include <NERvGear/interface/IData.h>
#include <NERvGear/template.h>

#include <string>

/// ----------------------------------------------------------------------------
/// \class DataSource
/// ----------------------------------------------------------------------------
class HttpData : public NERvGear::ImplT<NVG_ARGS_ITF_UID(IData)> {
public:

	//子Value枚举
	enum VALUE_LIST {
		STR_JSON_VALUE,
		STR_TEXT_VALUE,

		//用于计数子Value种类
		VALUE_COUNT
	};

	//计量单位枚举
	//enum UNIT_TYPE {
	//	UNIT_B,
	//	UNIT_KB,
	//	UNIT_MB,
	//	UNIT_GB,

	//	//用于计数计量单位
	//	UNIT_COUNT
	//};

	static const NERvGear::UID ID;

	/// Overridden Functions
	/// --------------------------------------------------------------
	/// 详情见HttpData.cpp

	virtual long NVG_METHOD GetId(NERvGear::UID* id);

	virtual size_t NVG_METHOD GetName(size_t len, wchar_t* name);
	virtual size_t NVG_METHOD GetDescrip(size_t len, wchar_t* descrip);
	virtual long NVG_METHOD QuerySource(NERvGear::IDataSource** source);

	virtual unsigned NVG_METHOD GetValueCount();

	virtual size_t NVG_METHOD GetValue(unsigned index, size_t nbyte, void* buf);
	virtual size_t NVG_METHOD GetMaximum(unsigned index, size_t nbyte, void* buf);
	virtual size_t NVG_METHOD GetMinimum(unsigned index, size_t nbyte, void* buf);

	virtual unsigned NVG_METHOD GetValueIndex(int id);
	virtual int NVG_METHOD GetValueId(unsigned index);
	virtual NERvGear::DATA::TYPE NVG_METHOD GetValueType(unsigned index);
	virtual size_t NVG_METHOD GetValueUnit(unsigned index, size_t len, wchar_t* unit);
	virtual size_t NVG_METHOD GetValueName(unsigned index, size_t len, wchar_t* name);
	virtual size_t NVG_METHOD GetValueDescrip(unsigned index, size_t len, wchar_t* descrip);

	virtual unsigned NVG_METHOD GetUpdateInterval(unsigned index);
	virtual long NVG_METHOD Update(unsigned index = NVG_ANY, const wchar_t* param = NULL);
	virtual long NVG_METHOD Invoke(unsigned index, const wchar_t* param = NULL, NERvGear::UI::IWindow* window = NULL);
	virtual size_t NVG_METHOD Config(unsigned index, NERvGear::DATA::CONFIG type, NERvGear::UI::IWindow* window, size_t len, wchar_t* param);

	virtual long NVG_METHOD SetValueUnit(unsigned index, const wchar_t* unit);
	virtual size_t NVG_METHOD EnumValueUnit(unsigned index, size_t len, wchar_t* units);

	virtual long NVG_METHOD SetEventListener(NERvGear::IEventHandler* listener);

	/// Constructors & Destructor
	/// --------------------------------------------------------------

	HttpData(DataSource* source);
	~HttpData();

private:

	//父Data Source
	DataSource* m_source;

	//请求参数
	std::wstring m_config;
	//返回数据
	std::wstring m_cur1;

	//Value2所用变量
	unsigned m_cur2;
	unsigned m_max2;
	//UNIT_TYPE m_unit2;

};

#endif // HttpData_H
