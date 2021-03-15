#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

/* Useful constants. */
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

/* Utility functions. */
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/* Commonly used headers. */
#include "ray.h"
#include "vec3.h"

#endif
