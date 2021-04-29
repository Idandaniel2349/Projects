//
// Created by sagig on 11/21/2020.
//
#pragma once
#include "string"
#include "vector"
#include "GISNamedTypes.h"

using std::string;
using std::vector;

/**
 * A Struct to load Basic Entity Info in Load Function
 */
struct BasicEntityInfo{
    EntityId id;
    string name;
    string description;
    vector<string> categoryTags;

    BasicEntityInfo(): id(EntityId("")){}

    BasicEntityInfo(const EntityId& _id, const string& _name, const string& _description, const vector<string>& _categoryTags):
    id(_id), name(_name), description(_description), categoryTags(_categoryTags){}
};