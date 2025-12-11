#pragma once
#include <string>
#include <map>
#include "set.h"
#include "stack.h"
#include "queue.h"
#include "hash.h"
#include "tree.h"

using std::string;
using std::map;

struct Database {
    map<string, Set>         sets;    
    map<string, Stack>       stacks;   
    map<string, Queue>       queues;
    map<string, Hash>        hashes;
    map<string, BinaryTree>  trees;

    bool load(const string& path);

    bool save(const string& path);
};
