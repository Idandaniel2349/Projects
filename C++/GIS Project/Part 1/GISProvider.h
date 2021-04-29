#pragma once

#include "GIS.h"

class GISProvider {
	GISProvider() {}	

public: 
	static GIS getGIS() {
		//You may change the implemntation below
		//(e.g. GIS may be a singelton)
		return GIS{};
	}
};