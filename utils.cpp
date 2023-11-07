#include "utils.h"

void utils::limit(int &x, int low, int high) {
    if (x > high)
        x = high;
    if (x < low)
        x = low;
}
