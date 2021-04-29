//
// Created by idan on 11/12/2020.
//

#pragma once

#include "string"
#include "unordered_set"
#include "algorithm"
#include "../MapEntities/Way.h"


using std::string;
using std::vector;
using std::remove_if;
using std::unordered_set;
/**
 * Restrictions class to set certain Restrictions
 *
 */
class Restrictions {
    unordered_set<string> restrictions;

public:
    Restrictions(string _restriction);

    bool isHighway() const {return restrictions.contains("highway");};

    bool isToll() const {return restrictions.contains("toll");}

    /**
     * checks if a way have restrictions
     * @param way - the way to check
     * @return if way has restrictions
     */
    bool isWayRestricted(Way& way) const;
};

