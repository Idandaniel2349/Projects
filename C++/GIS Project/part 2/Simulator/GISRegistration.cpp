#include "../Common/GISRegistration.h"
#include "SOLoader.h"

GISRegistration::GISRegistration(std::function<std::unique_ptr<AbstractGIS>()> factory) {
    SOLoader::getInstance().setAbsGis(factory);
}
