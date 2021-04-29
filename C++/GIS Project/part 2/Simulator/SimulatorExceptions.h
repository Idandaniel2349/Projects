//
// Created by sagig on 1/5/2021.
//

#pragma once

#include "string"
#include "stdexcept"

using std::string;

struct InvalidArgsException : std::runtime_error{
    InvalidArgsException(const string& msg): std::runtime_error(msg) {}
};

struct SOLoadFailedException : std::runtime_error{
    SOLoadFailedException(const string& msg): std::runtime_error(msg) {}
};

struct FailedOpenCSVException : std::runtime_error{
    FailedOpenCSVException(const string& msg): std::runtime_error(msg) {}
};