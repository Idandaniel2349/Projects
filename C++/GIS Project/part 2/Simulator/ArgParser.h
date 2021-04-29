//
// Created by sagig on 1/5/2021.
//
#pragma once
#include "unordered_map"
#include "string"
#include "optional"

using std::unordered_map;
using std::string;
using std::optional;

/**
 * ArgParser Class responsible for parsing the main arguments
 */
class ArgParser {
    unordered_map<string, string> args;
public:
    /**
     * The function sets the arguments in the class in preparation for the parsing
     * @param argv - the arguments to parse
     * @param argc -  the number of arguments to parse
     */
    void parseArgs(char* argv[], int argc);

    /**
     * The function gets a key on what value in needs to find and returns it from among the set arguments
     * @param key - the key to find
     * @return - the correct string value from the set ones
     */
    optional<string> getStringVal(const string& key);

    /**
     *The function gets a key on what value in needs to find and returns it from among the set arguments
     * @param key - the key to find
     * @return - the correct int value from the set ones
     */
    optional<int> getIntVal(const string& key);

    string getStringValOrThrow(const string& key);
};

