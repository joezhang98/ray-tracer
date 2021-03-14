#ifndef MATERIALH
#define MATERIALH
#include <random>
#include "hittable.h"

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered,
                         std::uniform_real_distribution<>& dis,
                         std::mt19937& gen) const = 0;
};

#endif
