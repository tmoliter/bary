#include "utils.h"

void utils::limit(int &x, int low, int high) {
    if (x > high)
        x = high;
    if (x < low)
        x = low;
}

void utils::parse_ints(vector<int*> ints, ifstream &mapData) {
    string value = "";
    char current;
    for (auto i : ints) {
        mapData.get(current);
        if (current == '|')
            continue;
        if (current == '*') 
            break;
        while (current != ',' && current != '>') {
            value.push_back(current);
            mapData.get(current);
        }
        *i = std::stoi(value);
        value.clear();
    }
}

void utils::parse_strings(vector<string*> strings, ifstream &mapData) {
    string value = "";
    char current;
    for (auto s : strings) {
        mapData.get(current);
        if (current == '|')
            continue;
        if (current == '*') 
            break;
        while (current != ',' && current != '>') {
            value.push_back(current);
            mapData.get(current);
        }
        *s = value;
        value.clear();
    }
}