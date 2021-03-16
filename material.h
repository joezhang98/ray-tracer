#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "util.h"

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
                         color& attenuation, ray& scattered) const = 0;
};

/*
   A diffuse material.
*/
class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const override {
        auto scatter_direction = rec.normal + random_unit_vector();

        /* Handle degenerate scatter direction. */
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

/*
   A metal material.
*/
class metal : public material {
public:
    metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    virtual bool scatter(const ray&r_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

        /* Add fuzzy reflection. */
        scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());

        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

/*
   A dielectric material that refracts when possible.
*/
class dielectric : public material {
public:
    dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         color& attenuation, ray& scattered) const override {
        double refraction_ratio = rec.front_face ? (1.0/ir) : ir;

        /* Dielectric material does not absorb light. */
        attenuation = color(1.0, 1.0, 1.0);

        /* Determine if refraction is possible. */
        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        /* Get direction of refracted or reflected ray. */
        vec3 direction;
        if (cannot_refract)
            direction = reflect(unit_direction, rec.normal);
        else   
            direction = refract(unit_direction, rec.normal, refraction_ratio);

        scattered = ray(rec.p, direction);
        return true;
    }

public:
    double ir;
};

#endif
