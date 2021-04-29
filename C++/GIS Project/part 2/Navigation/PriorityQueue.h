//
// Created by sagig on 12/20/2020.
//

#pragma once

#include "queue"

using std::priority_queue;

template<typename T, typename priority_t>
/**
 * A class representing a priorityQueue object for A* algorithm
 */
struct PriorityQueue{
    typedef std::pair<priority_t, T> PQElement;
    std::priority_queue<PQElement, std::vector<PQElement>,
            std::greater<PQElement>> elements;

    inline bool empty() const {
        return elements.empty();
    }

    inline void put(T item, priority_t priority) {
        elements.emplace(priority, item);
    }

    T get() {
        T best_item = elements.top().second;
        elements.pop();
        return best_item;
    }
};
