#ifndef MATERIAL_TYPESH
#define MATERIAL_TYPESH
#include "material.h"

/* Selects and returns a random point in a unit radius sphere
   centered at the origin. */
vec3 random_in_unit_sphere(std::uniform_real_distribution<>& dis,
                           std::mt19937& gen) {
    vec3 p;
    do {
        p = 2.0 * vec3(dis(gen), dis(gen), dis(gen)) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    
    return p;
}

/* Given an incident ray with direction V and a surface normal N,
   returns the direction of the reflected ray. */
vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

/* Given an incident ray with direction V, a surface normal N, and
   a ratio of refractive indices NI_OVER_NT, determines the direction
   of the refracted ray (if any), and stores it in REFRACTED. Returns
   true if there is a refracted ray and false otherwise. */
bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt*dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n*dt) - n * sqrt(discriminant);
        return true;
    }
    else
        return false;
}

/*
   A diffuse material.
*/
class lambertian : public material {
public:
    lambertian(const vec3& a) : albedo(a) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered,
                         std::uniform_real_distribution<>& dis,
                         std::mt19937& gen) const {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere(dis, gen);
        scattered = ray(rec.p, target-rec.p);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

/*
   A metal material.
*/
class metal : public material {
public:
    metal(const vec3& a, float f) : albedo(a) {
        if (f < 1)
            fuzz = f;
        else
            fuzz = 1;
    }
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered,
                         std::uniform_real_distribution<>& dis,
                         std::mt19937& gen) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);

        /* Randomize reflected direction using FUZZ. */
        scattered = ray(rec.p, reflected +
                        fuzz * random_in_unit_sphere(dis, gen));

        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    float fuzz;
};

/*
   A dielectric material.
*/
class dielectric : public material {
public:
    dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const ray& r_in, const hit_record& rec,
                         vec3& attenuation, ray& scattered,
                         std::uniform_real_distribution<>& dis,
                         std::mt19937& gen) const {
        vec3 outward_normal;
        float ni_over_nt;
        vec3 refracted;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        attenuation = vec3(1.0, 1.0, 1.0);

        /* Set OUTWARD_NORMAL and NI_OVER_NT depending on whether ray
           R_IN originates from outside or inside the object. */
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
        }

        /* Set SCATTERED depending on whether ray R_IN is refracted
           or reflected after hitting the material. */
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            scattered = ray(rec.p, refracted);
        }
        else {
            scattered = ray(rec.p, reflected);
            return false;
        }
        return true;
    }

    float ref_idx;
};

#endif
