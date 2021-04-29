//
// Created by sagig on 1/5/2021.
//
#include "ArgParser.h"
#include "SimulatorExceptions.h"

void ArgParser::parseArgs(char* argv[], int argc){
    if(argc % 2 == 0){
        throw InvalidArgsException("Invalid amount of cmd args");
    }
    string currentKey;
    for (int i = 1; i < argc; ++i) {
        if(i % 2){
            if(*argv[i] == '-'){//set as the current key
                string val = argv[i];
                currentKey = val.substr(1);
            }
            else{
                throw InvalidArgsException("Missing - in arg");
            }
        }
        else{//insert value of the current key
            args[currentKey] = argv[i];
        }
    }
}

optional<string> ArgParser::getStringVal(const string& key){
    if(args.contains(key)){
        return args[key];
    }
    return optional<string>();
}

optional<int> ArgParser::getIntVal(const string& key){
    if(args.contains(key)){
        return std::stoi(args[key]);
    }
    return optional<int>();
}

string ArgParser::getStringValOrThrow(const string& key){
    if(args.contains(key)){
        return args[key];
    }
    throw InvalidArgsException("CMD args did not contain " + key);
}