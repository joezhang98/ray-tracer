#ifndef HITTABLEH
#define HITTABLEH
#include "ray.h"

/* Reference to the material class to avoid circular references since
   hittable objects and materials need to know about each other. */
class material;

/* A hit record contains information about the point of intersection
   between a ray and an object. */
struct hit_record {
    float t;            /* The relative distance along the ray of the hit. */
    vec3 p;             /* The point of intersection. */
    vec3 normal;        /* The surface normal at the point of intersection. */
    material *mat_ptr;  /* A reference to the object material type. */
};

/*
   An abstract class for anything that a ray might intersect with or
   hit (hence "hittable").
*/
class hittable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
