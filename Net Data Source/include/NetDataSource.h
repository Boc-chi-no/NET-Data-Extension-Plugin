
#ifndef NetDataSource_H
#define NetDataSource_H

#include <NERvGear/plugin.h>

class NetDataSource : public NERvGear::PluginImpl {
public:

    /// Handlers
    /// -----------------

    // implementation of IPlugin

    virtual long NVG_METHOD OnInitial();
    virtual long NVG_METHOD OnReady();
    virtual long NVG_METHOD OnRelease();

    NVG_DECLARE_PLUGIN(NetDataSource)
};

#endif // NetDataSource_H
