//
// Created by sagig on 11/20/2020.
//

#include "Logger.h"

//Logs given messages into the Logger file
void Logger::log(const string& message) {

    if(!logFile.is_open()){//open only if not already open
        logFile.open(path);
    }

    if(logFile){
        logFile<<message<<std::endl;
        return;
    }
    logFile.open(path);
    //try to open again
    if(logFile){
        logFile<<message<<std::endl;
    }
}

Logger::~Logger(){
    if(logFile.is_open()){
        logFile.close();
    }
}