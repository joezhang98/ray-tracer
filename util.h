#ifndef UTIL_H
#define UTIL_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

/*
   Useful constants.
*/
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

/* 
   Utility functions.
*/

/* Converts degrees to radians. */
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

/* Returns a random real in the range [0, 1). */
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

/* Returns a random real in the range [MIN, MAX). */
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

/* Returns a random integer in the range [MIN, MAX]. */
inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max+1));
}

/* Clamps value X to the range [MIN, MAX]. */
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

/*
   Commonly used headers.
*/
#include "ray.h"
#include "vec3.h"

#endif
