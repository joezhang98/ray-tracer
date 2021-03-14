#ifndef RAYH
#define RAYH
#include "vec3.h"

/*
   A ray class for computing the color seen along a particular ray.
   Rays are represented as an origin A and a direction B.
*/
class ray {
public:
    ray() {}
    ray(const vec3& a, const vec3& b) { A = a; B = b; }
    vec3 origin() const { return A; }
    vec3 direction() const { return B; }
    vec3 point_at_parameter(float t) const { return A + t*B; }

    vec3 A;
    vec3 B;
};

#endif
