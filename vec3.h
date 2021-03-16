#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

/*
   A vector class for storing geometric vectors and colors. For
   geometry, the coordinates represent x, y, z values. For color,
   the coordinates represent R, G, and B values.
*/
class vec3 {
public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }
    double r() const { return e[0]; }
    double g() const { return e[1]; }
    double b() const { return e[2]; }
    
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t) {
        return *this *= 1/t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    /* Returns a random vector with values in the range [0, 1). */
    inline static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    /* Returns a random vector with values in the range [MIN, MAX). */
    inline static vec3 random(double min, double max) {
        return vec3(random_double(min, max),
                    random_double(min, max),
                    random_double(min, max));
    }

    /* Returns true if the vector is near zero in all dimensions. */
    inline bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(e[0]) < s) &&
               (fabs(e[1]) < s) &&
               (fabs(e[2]) < s);
    }

public:
    double e[3];
};

/* Type aliases for vec3 to make usage clearer. */
using point3 = vec3;
using color = vec3;

inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
    return out;
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return (u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]);
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3((u.e[1] * v.e[2] - u.e[2] * v.e[1]),
                (u.e[2] * v.e[0] - u.e[0] * v.e[2]),
                (u.e[0] * v.e[1] - u.e[1] * v.e[0]));
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

/*
   Functions for computing reflection and refraction of incident rays
   off surfaces with different material types.
*/

/* Returns a random point in a unit radius sphere with center
    at the origin (approximation to Lambertian diffuse). */
inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() >= 1)
            continue;

        return p;
    }
}

/* Returns a random point on the surface of the unit sphere with
   center at the origin (Lambertian diffuse). */
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

/* Returns a point based on a uniform scatter direction for all
   angles (i.e., no dependence on the angle from the normal). */
inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else   
        return -in_unit_sphere;
}

/* Given an incident ray with direction V and a surface normal N,
   returns the direction of the reflected ray. */
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n)*n;
}

/* Given an incident ray with direction UV, a surface normal N, and
   a ratio of refractive indices NI_OVER_NT, returns the direction of
   the refracted ray. */
inline vec3 refract(const vec3& uv, const vec3& n, double ni_over_nt) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = ni_over_nt * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

#endif
