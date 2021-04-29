#pragma once 

#include "../GIS.h"

/**
 * @brief NavigationGIS class 
 *        The class below would be provided by the course staff, see Dependencies comment below
 *        This is a Proxy class for your GIS, the purpose of it is to:
 *        [1] restrict the navigation system to use only certain methods of the GIS class
 *        [2] allow monitoring the calls to GIS by the navigation system via this proxy
 * 
 */
class NavigationGIS {
  const GIS& gis;

public:
  /**
   * @brief Construct a new Navigation G I S object
   * 
   * @param gis 
   */
  NavigationGIS(const GIS& gis): gis(gis) {}

  /**
   * @brief Get the Ways By Junction object
   * 
   * @param junctionId 
   * @return std::vector<EntityId> 
   */
  std::vector<EntityId> getWaysByJunction(const EntityId& junctionId) const {
  	return gis.getWaysByJunction(junctionId);
  }

  /**
   * @brief Get the Way Closest Point object
   * 
   * @param coords 
   * @return std::pair<Coordinates, EntityId> 
   */
  std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates& coords) const {
  	return gis.getWayClosestPoint(coords);
  }

  /**
   * @brief Get the Way Closest Point object
   * 
   * @param coords 
   * @param res 
   * @return std::pair<Coordinates, EntityId> 
   */
  std::pair<Coordinates, EntityId> getWayClosestPoint(const Coordinates& coords, const Restrictions& res) const {
    return gis.getWayClosestPoint(coords, res);
  }

  /**
   * @brief Get the Way object
   * 
   * @param wayId 
   * @return const Way& 
   */
  const Way& getWay(const EntityId& wayId) const {
	  return gis.getWay(wayId); 
  }
};