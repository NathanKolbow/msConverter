#ifndef SIMSUITE_CORE_HELPERS_HPP_
#define SIMSUITE_CORE_HELPERS_HPP_

#include <cmath>

inline bool equalDoubles(double a, double b) {
    return std::abs(a - b) < 1e-4;
}

#endif