#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

/* A hit record contains information about the point of intersection
   between a ray and an object. */
struct hit_record {
    point3 p;         /* The point of intersection. */
    vec3 normal;      /* The surface normal at the point of intersection. */
    double t;         /* The relative distance along the ray of the hit. */
    bool front_face;  /* True if ray outside, false if ray inside. */

    /* Sets OUTWARD_NORMAL based on direction of ray R. */
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/*
   An abstract class for anything that a ray might intersect with or
   hit (hence "hittable").
*/
class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const = 0;
};

#endif
