#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <fstream>

using namespace std;

namespace utils {
    void limit(int &x, int low, int high);
    void parse_ints(vector<int*> ints, ifstream &mapData);
    void parse_strings(vector<string*> strings, ifstream &mapData);
};

#endif
