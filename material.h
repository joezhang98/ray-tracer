#ifndef MATERIALH
#define MATERIALH
#include <random>
#include "hittable.h"

/*
   An abstract material class to represent different materials of
   objects. The scatter function encapsulates the different
   behaviors of a ray when it hits materials of different types.

   SCATTERED denotes the scattered ray (if any), and ATTENUATION
   denotes the degree of attenuation for the scattered ray.
*/
class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered,
                         std::uniform_real_distribution<>& dis,
                         std::mt19937& gen) const = 0;
};

#endif
