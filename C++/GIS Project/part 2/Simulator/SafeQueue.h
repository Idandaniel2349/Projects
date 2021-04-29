//
// Created by idan on 04/01/2021.
//

#pragma once

#include "queue"
#include "mutex"
#include "optional"

using std::queue;
using std::mutex;
using std::optional;
using std::lock_guard;

/**
 * A class representing a thread safe queue
 */
template<class T> class SafeQueue {
    queue<T> baseQueue;
    mutex lock;

public:
    void push(const T & elem){
        lock_guard<std::mutex> guard(lock);
        baseQueue.push(elem);
    }

    optional<T> pop(){
        lock_guard<std::mutex> guard(lock);
        if (!baseQueue.empty()) {
            auto res = optional<T>(baseQueue.front());
            baseQueue.pop();
            return res;
        } else {
            return optional<T>();
        }
    }

    bool isEmpty(){
        lock_guard<std::mutex> guard(lock);
        return baseQueue.empty();
    }
};



