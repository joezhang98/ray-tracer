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

/*
   A class for translated objects, implemented by shifting the
   incident rays relative to the object.
*/
class translate : public hittable {
public:
    translate(shared_ptr<hittable> p, const vec3& displacement)
        : ptr(p), offset(displacement) {}

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override;
   
public:
    shared_ptr<hittable> ptr;
    vec3 offset;
};

/* Compute ray-object intersection with a ray shifted by offset in
   the opposite direction, then add it back in the hit record. */
bool translate::hit(const ray& r, double t_min, double t_max,
                    hit_record& rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);

    return true;
}

/* Construct the bounding box around the original object and then
   apply the offset. */
bool translate::bounding_box(double time0, double time1,
                             aabb& output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(output_box.min() + offset, output_box.max() + offset);
    
    return true;
}

/*
   a class for objects rotated counter-clockwise about Y.
*/
class rotate_y : public hittable {
public:
    rotate_y(shared_ptr<hittable> p, double angle);

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override {
    output_box = bbox;
    return hasbox;
    }

public:
    shared_ptr<hittable> ptr;
    double sin_theta;
    double cos_theta;
    bool hasbox;
    aabb bbox;
};

rotate_y::rotate_y(shared_ptr<hittable> p, double angle) : ptr(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    point3 min(infinity, infinity, infinity);
    point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                auto newx = cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, double t_min, double t_max,
                   hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

    normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

#endif
