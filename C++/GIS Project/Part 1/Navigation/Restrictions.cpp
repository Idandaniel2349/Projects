//
// Created by idan on 11/12/2020.
//

#include "Restrictions.h"

Restrictions::Restrictions(string _restriction) {
    _restriction.erase(remove_if(_restriction.begin(),_restriction.end(),isspace),_restriction.end());

    size_t pos = 0;
    std::string token;
    bool enteredWhile = false;
    while ((pos = _restriction.find(',')) != std::string::npos) {
        enteredWhile = true;
        token = _restriction.substr(0, pos);
        restrictions.insert(token);
        _restriction.erase(0, pos + 1);
    }
    if(!enteredWhile){
        restrictions.insert(_restriction);
    }
}

bool Restrictions::isWayRestricted(Way& way) const{
    if(isHighway()){
        return way.isHighway();
    }
    if(isToll()){
        return way.isToll();
    }
    return false;
}