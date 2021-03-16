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
   metal(const color& a) : albedo(a) {}

   virtual bool scatter(const ray&r_in, const hit_record& rec,
                        color& attenuation, ray& scattered) const override {
      vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
      scattered = ray(rec.p, reflected);
      attenuation = albedo;
      return (dot(scattered.direction(), rec.normal) > 0);
   }

public:
   color albedo;
};

#endif
