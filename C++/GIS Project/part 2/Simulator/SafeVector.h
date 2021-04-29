//
// Created by idan on 04/01/2021.
//

#pragma once

#include "vector"
#include "mutex"

using std::vector;
using std::mutex;
using std::lock_guard;

/**
 * A class representing a thread safe vector
 */
template<class T> class SafeVector {
    vector<T> vec;
    mutex lock;
public:
    void pushBack(const T &elem){
        lock_guard<std::mutex> guard(lock);
        vec.push_back(elem);
    }

    const vector<T> &getVec() const {return vec;}
};


