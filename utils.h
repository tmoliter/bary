#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <string>
#include <fstream>
#include "luaUtils.h"

using namespace std;

namespace utils {
    void limit(int &x, int low, int high);
    inline string LOREM_IPSUM = "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here', making it look like readable English. Many desktop publishing packages and web page editors now use Lorem Ipsum as their default model text, and a search for 'lorem ipsum' will uncover many web sites still in their infancy. Various versions have evolved over the years, sometimes by accident, sometimes on purpose (injected humour and the like).";
};

#endif
