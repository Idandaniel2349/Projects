//
// Created by idan on 11/12/2020.
//

#pragma once

#include <stdexcept>
#include "string"

using std::string;

struct IdNotFoundException : std::runtime_error{
    IdNotFoundException(const char* msg): std::runtime_error(msg) {}

    IdNotFoundException(const string& msg): std::runtime_error(msg) {}
};

struct NotAWayException : std::runtime_error{
    NotAWayException(const char* msg): std::runtime_error(msg) {}

    NotAWayException(const string& msg): std::runtime_error(msg) {}
};
