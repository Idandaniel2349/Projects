//
// Created by idan on 10/12/2020.
//

#pragma once

#include "string"
#include "Route.h"

/**
 * @brief Routes class
 *
 */
class Routes {
    bool valid;
    Route shortDistance;
    Route shortTime;
    string errorMessage;
public:
    Routes(const bool& _valid, const Route& _shortDistance, const Route& _shortTime): valid(_valid),
    shortDistance(_shortDistance),shortTime(_shortTime) {};

    Routes(const bool& _valid, const Route& _shortDistance, const Route& _shortTime, const string& _errorMessage): valid(_valid),
    shortDistance(_shortDistance),shortTime(_shortTime), errorMessage(_errorMessage) {};

    /**
     * @brief
     *
     * @return true - Routes are valid
     * @return false - Routes not valid
     */
    bool isValid() const {return valid;} // is route valid

    // following functions can be called only if isValid is true - undefined otherwise

    /**
    * @brief Retuns Route with shortest total distance
    *        Note: function can be called only if isValid is true - otherwise behavior is undefined
    *
    * @return const Route& - shortest route in distance
    */
    const Route& shortestDistance() const {return shortDistance;};

    /**
     * @brief Retuns fastest Route
     *        Note: function can be called only if isValid is true - otherwise behavior is undefined
     *
     * @return const Route& - shortest route in time
     */
    const Route& shortestTime() const {return shortTime;};

    // following function can be called only if isValid is false - undefined otherwise

    /**
     * @brief Get the Error Message
     *        Note: function can be called only if isValid is false - otherwise behavior is undefined
     *
     * @return const std::string& - error message
     */
    const std::string& getErrorMessage() const{return errorMessage;}
};
