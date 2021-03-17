#ifndef SPHERE_H
#define SPHERE_H

#include <cmath>
#include "hittable.h"
#include "vec3.h"

/*
   A sphere object represented by a center CEN and radius R.
*/
class sphere : public hittable {
public:
    sphere() {}
    sphere(point3 cen, double r, shared_ptr<material> m)
        : center(cen), radius(r), mat_ptr(m) {};

    virtual bool hit(const ray&r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override;
    
public:
    point3 center;                 /* Sphere center. */
    double radius;                 /* Sphere radius. */
    shared_ptr<material> mat_ptr;  /* Reference to sphere material. */

private:

    /* Computes the U, V coordinates of a point P on a unit radius
       sphere with center at the origin, used for texture lookups. */
    static void get_sphere_uv(const point3& p, double& u, double& v) {

        /* THETA is the angle up from the bottom pole (negative y)
           and PHI is the angle around the y-axis. */
        auto theta = acos(-p.y());
        auto phi = atan2(-p.z(), p.x()) + pi;

        u = phi / (2*pi);
        v = theta / pi;
    }
};

/* Solve for ray-sphere intersection using substitution of the ray
   equation for ray R into the equation of a sphere and applying
   the quadratic formula. T_MIN and T_MAX dictate the interval for
   a valid hit, and properties of the intersection point (if any)
   are stored in REC. */
bool sphere::hit(const ray& r, double t_min, double t_max,
                 hit_record& rec) const {
    vec3 oc = r.origin() - center;
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
    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;

    return true;
}

/* Constructs a bounding box for the sphere and stores it in
   OUTPUT_BOX. Returns true. */
bool sphere::bounding_box(double time0, double time1,
                          aabb& output_box) const {
    output_box = aabb(center - vec3(radius, radius, radius),
                      center + vec3(radius, radius, radius));
    return true;
}

#endif
