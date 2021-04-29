#pragma once 

#include "AbstractNavigation.h"
#include "NavigationGIS.h"
#include <memory> 
#include <functional>

class NavigationRegistration {
public:
  NavigationRegistration(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS&)>);
};

#define REGISTER_NAVIGATION(class_name) \
NavigationRegistration register_me_##class_name \
	([](const NavigationGIS& navigation_gis){return std::make_unique<class_name>(navigation_gis);} );

