#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.h"
#include "util.h"

/* Reference to the material class to avoid circular references since
   hittable objects and materials need to know about each other. */
class material;

/* A hit record contains information about the point of intersection
   between a ray and an object. */
struct hit_record {
    point3 p;                      /* The point of intersection. */
    vec3 normal;                   /* The surface normal at P. */
    double t;                      /* The relative distance along the ray. */
    double u;                      /* U coordinate for texture lookups. */
    double v;                      /* V coordinate for texture lookups. */
    bool front_face;               /* True if ray outside, false if inside. */
    shared_ptr<material> mat_ptr;  /* Reference to object material type. */

    /* Sets OUTWARD_NORMAL based on direction of ray R. */
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

/*
   An abstract class for anything enclosed in a bounding box that a
   ray might intersect with or hit (hence "hittable").
*/
class hittable {
public:
    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const = 0;            
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const = 0;
};

#endif
