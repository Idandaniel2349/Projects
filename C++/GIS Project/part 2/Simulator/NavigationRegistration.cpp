#include "../Common/NavigationRegistration.h"
#include "SOLoader.h"

NavigationRegistration::NavigationRegistration(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS&)> factory) {
    SOLoader::getInstance().setNavigationFactory(factory);
}
