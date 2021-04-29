#pragma once 

#include <vector>
#include "../GISNamedTypes.h"

/**
 * @brief Route class
 * 
 */
class Route {
    std::vector<std::pair<EntityId, Direction>> ways;
    Coordinates startPoint;
    Coordinates endPoint;
    Meters length;
    Minutes duration;
public:
    Route(const std::vector<std::pair<EntityId, Direction>> &_ways, const Coordinates &startPoint,
          const Coordinates &endPoint, const Meters &length, const Minutes &duration) : ways(_ways),
                                                                                        startPoint(startPoint),
                                                                                        endPoint(endPoint),
                                                                                        length(length),
                                                                                        duration(duration) {};

public:
    /**
     * @brief Get the ways constructing the Route and direction per way
     * 
     * @return const std::vector<std::pair<EntityId, Direction>>&
     */
    const std::vector<std::pair<EntityId, Direction>>& getWays() const{return ways;};

    /**
     * @brief Get the Way's Start Point 
     * 
     * @return const Coordinates& 
     */
    const Coordinates& getWayStartPoint() const{return startPoint;};
    
    /**
     * @brief Get the Way's End Point
     * 
     * @return const Coordinates& 
     */
    const Coordinates& getWayEndPoint() const{return endPoint;};
    
    /**
     * @brief Get Way's total length
     * 
     * @return Meters 
     */
    Meters totalLength() const{return length;};
    
    /**
     * @brief Get Way's estimated duration
     * 
     * @return Minutes 
     */
    Minutes estimatedDuration() const{return duration;};

    /**
     * compares two route objects
     * @param rhs
     * @return
     */
    bool operator==(const Route& rhs) const {
        return ways==rhs.ways && startPoint==rhs.startPoint && endPoint==rhs.endPoint && length==rhs.length
               && duration==rhs.duration;
    }
};

