#ifndef SPHEREH
#define SPHEREH
#include "hittable.h"

/*
   A sphere object represented by a center CEN and radius R, with
   material type determined by the reference MAT_PTR.
*/
class sphere : public hittable {
public:
    sphere() {}
    sphere(vec3 cen, float r, material *mat_ptr) :
        center(cen), radius(r), mat_ptr(mat_ptr) {};
    virtual bool hit(const ray&r, float t_min, float t_max,
                     hit_record& rec) const;
    vec3 center;
    float radius;
    material *mat_ptr;
};

/* Solve for ray-sphere intersection using substitution of the ray
   equation for ray R into the equation of a sphere and applying
   the quadratic formula. T_MIN and T_MAX dictate the interval for
   a valid hit, and properties of the intersection point (if any)
   are stored in REC. */
bool sphere::hit(const ray& r, float t_min, float t_max,
                 hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }

    return false;
}

#endif
