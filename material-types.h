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

/* Schlick polynomial approximation to simulate reflectivity that
   varies with angle for real glass. */
float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1+ref_idx);
    return r0*r0 + (1-r0*r0) * pow((1-cosine), 5);
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
   A dielectric material (with angle-varying reflectivity).
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

        /* Terms for the Schlick approximation. */
        float reflect_prob;
        float cosine;

        /* Set OUTWARD_NORMAL and NI_OVER_NT depending on whether ray
           R_IN originates from outside or inside the object. */
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) /
                r_in.direction().length();
        }
        else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) /
                r_in.direction().length();
        }

        /* Determine whether R_IN refracts or reflects and set
           REFLECT_PROB accordingly. */
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        }
        else {
            reflect_prob = 1.0;
        }

        /* Set SCATTERED taking into account REFLECT_PROB which
           reflects the fact that reflectivity varies with angle. */
        if (dis(gen) < reflect_prob) {
            scattered = ray(rec.p, reflected);
        }
        else {
            scattered = ray(rec.p, refracted);
        }

        return true;
    }

    float ref_idx;
};

#endif
