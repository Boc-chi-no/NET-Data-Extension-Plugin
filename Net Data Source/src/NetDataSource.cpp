#include "NetDataSource.h"
#include "DataSource.h"
#include "defs.h"

#include <NERvGear/NERvSDK.h>

using namespace NERvGear;
NVG_BEGIN_PLUGIN_INFO(NetDataSource)
NVG_DECLARE_PLUGIN_UID(0x2f94132f, 0x4489, 0x4375, 0xb2, 0x6e, 0xfd, 0x27, 0xbf, 0xb8, 0xa3, 0xcf) // {2f94132f-4489-4375-b26e-fd27bfb8a3cf}
    NVG_DECLARE_PLUGIN_TYPE(PLUGIN::BASIC)
    NVG_DECLARE_PLUGIN_VERSION(VER_MAJOR, VER_MINOR, VER_SUBMINOR)
    NVG_DECLARE_PLUGIN_NAME(NAME_STRING)
    NVG_DECLARE_PLUGIN_DESCRIP(DESCRIP_STRING)
    NVG_DECLARE_PLUGIN_COMMENT(COMMENT_STRING)
    NVG_DECLARE_PLUGIN_AUTHOR("L Jea")
    NVG_DECLARE_PLUGIN_HOMEPAGE("www.lioat.cn")
    NVG_DECLARE_PLUGIN_EMAIL("sino@lioat.cn")
NVG_END_PLUGIN_INFO(NetDataSource)

/// ----------------------------------------------------------------------------
/// component registration
/// ----------------------------------------------------------------------------

NVG_BEGIN_COMPONENT_REGISTER(NetDataSource)
NVG_REGISTER_OBJECT(DataSource, false) // no aggregation
NVG_END_COMPONENT_REGISTER()

/// ============================================================================
/// implementation
/// ============================================================================

/// ----------------------------------------------------------------------------
/// event handlers
/// ----------------------------------------------------------------------------

long NetDataSource::OnInitial()
{
	return PluginImpl::OnInitial();
}

long NetDataSource::OnRelease()
{
    return PluginImpl::OnRelease();
}

long NetDataSource::OnReady()
{
    return PluginImpl::OnReady();
}

NVG_IMPLEMENT_PLUGIN(NetDataSource)
