//
// Created by idan on 14/11/2020.
//

#include "POI.h"


string POI::accessibilityToString(accessibility a)
{
    static unordered_map<accessibility, string> mapper = {{accessibility::by_car, "by car"},
                                                          {accessibility::by_foot, "by foot"},
                                                          {accessibility::by_wheelchair, "by wheelchair"}
    };

    return mapper[a];
}

accessibility POI::stringToAccessibility(const string& s)
{
    static unordered_map<string, accessibility> mapper = {{"by car", accessibility::by_car},
                                                          {"by foot", accessibility::by_foot},
                                                          {"by wheelchair", accessibility::by_wheelchair}
    };
    return mapper.at(s);
}



