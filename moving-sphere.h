#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "aabb.h"
#include "hittable.h"
#include "util.h"

/*
   A sphere object represented by a center CEN and radius R.
*/
class moving_sphere : public hittable {
public:
    moving_sphere() {}
    moving_sphere(point3 cen0, point3 cen1, double _time0, double _time1,
                  double r, shared_ptr<material> m)
        : center0(cen0), center1(cen1), time0(_time0), time1(_time1),
        radius(r), mat_ptr(m) {};

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double _time0, double _time1,
                              aabb& output_box) const override;
                    
    point3 center(double time) const;

public:
    point3 center0, center1;       /* Beginning and end center of sphere. */
    double time0, time1;           /* (time0, center0), (time1, center1). */
    double radius;                 /* Sphere radius. */
    shared_ptr<material> mat_ptr;  /* Reference to sphere material. */
};

/* Returns the center of the sphere at TIME, given that the center
   is at center0 at time0 and moves linearly to center1 at time1. */
point3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

/* Solve for ray-sphere intersection using substitution of the ray
   equation for ray R into the equation of a sphere and applying
   the quadratic formula. T_MIN and T_MAX dictate the interval for
   a valid hit, and properties of the intersection point (if any)
   are stored in REC. 
   
   Since this is a moving sphere, use center(time) with the ray time
   to get the correct intersection (if any). */
bool moving_sphere::hit(const ray& r, double t_min, double t_max,
                        hit_record& rec) const {
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;
    
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0)
        return false;

    /* Find closest point of intersection in range T_MIN to T_MAX. */
    auto sqrtd = sqrt(discriminant);
    auto root = (-half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root)
            return false;
    }

    /* Store information about point of intersection in REC. */
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center(r.time())) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;

    return true;
}

/* Constructs a bounding box for the moving sphere and stores it in
   OUTPUT_BOX. Returns true. */
bool moving_sphere::bounding_box(double _time0, double _time1,
                                 aabb& output_box) const {
    aabb box0(center(_time0) - vec3(radius, radius, radius),
              center(_time0) + vec3(radius, radius, radius));
    aabb box1(center(_time1) - vec3(radius, radius, radius),
              center(_time1) + vec3(radius, radius, radius));
    output_box = surrounding_box(box0, box1);
    return true;
}

#endif
