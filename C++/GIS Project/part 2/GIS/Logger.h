//
// Created by sagig on 11/20/2020.
//
#pragma once
#include "iostream"
#include "fstream"
#include "string"

using std::ofstream;
using std::string;

/**
 * A class representing a Logger
 */
class Logger {
    string path;
    ofstream logFile;

public:
    explicit Logger(const string& _path): path(_path){}

    ~Logger();

    Logger(const Logger& other) : Logger(other.path){
    }

    Logger(Logger&& other)  = default;

    Logger& operator=(const Logger& other){
        if (this != &other) { // self-assignment check expected
            path = other.path;
            logFile.close();
        }
        return *this;
    }

    Logger& operator=(Logger&& other) = default;

    //Logs given messages into the Logger file
    /**
   * Logs given messages into the Logger file
   * @param message - the given message to log
   */
    void log(const string& message) ;
};



