#ifndef DataSource_H
#define DataSource_H

#include <NERvGear/interface/IDataSource.h>

#include <NERvGear/object.h>

/// ----------------------------------------------------------------------------
/// \class DataSource
/// ----------------------------------------------------------------------------
class DataSource : public NERvGear::IDataSource {
public:

	// We are providing 2 data in this data source,
	// this enumerator help us to index and count these data.
	enum DATA_LIST {
		DATA_DATA_1,
		DATA_DATA_2,

		//用于计数子Data种类
		DATA_COUNT
	};

    /// Overridden Functions
    /// --------------------------------------------------------------
	/// 详情见DataSource.cpp

    virtual long NVG_METHOD GetId(NERvGear::UID* id);

    virtual size_t NVG_METHOD GetName(size_t len, wchar_t* name);
    virtual size_t NVG_METHOD GetDescrip(size_t len, wchar_t* descrip);

    virtual unsigned NVG_METHOD GetDataCount();

    virtual long NVG_METHOD GetData(unsigned index, NERvGear::IData** data);
    virtual long NVG_METHOD FindData(const NERvGear::UID& id, NERvGear::IData** data);

    /// Constructors & Destructor
    /// --------------------------------------------------------------

    DataSource();	// Component object accepts default constructor only.
    virtual ~DataSource();

private:

    /// Variables
    /// -----------------

    NVG_DECLARE_OBJECT(DataSource)
};

#endif // DataSource_H
