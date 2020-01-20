#include "mcuplugin_plugin.h"
#include "mcuinfo.h"

#include <qqml.h>

void McupluginPlugin::registerTypes(const char *uri)
{
    // @uri com.mycompany.qmlcomponents
    qmlRegisterType<McuInfo>(uri, 1, 0, "McuInfo");
}

