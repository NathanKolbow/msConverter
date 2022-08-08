#ifndef SIMSUITE_CORE_HELPERS_HPP_
#define SIMSUITE_CORE_HELPERS_HPP_

#include <cmath>

inline bool equalDoubles(double a, double b) {
    // return a == b;
    return std::abs(a - b) < 1e-6;
}

#endif