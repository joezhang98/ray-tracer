#ifndef CAMERAH
#define CAMERAH
#include <random>
#include "ray.h"

/* Selects and returns a random point in a unit radius disk
   centered at the origin in the xy-plane. */
vec3 random_in_unit_disk(std::uniform_real_distribution<>& dis,
                         std::mt19937& gen) {
    vec3 p;
    do {
        p = 2.0 * vec3(dis(gen), dis(gen), 0) - vec3(1, 1, 0);
    } while (p.squared_length() >= 1.0);

    return p;
}

/*
   A camera class to encapsulate the axis-aligned camera with
   location at the origin and looking down the negative z-axis,
   with adjustable field of view (fov).

   lower_left_corner denotes the coordinate of the lower left
   corner of the screen, and horizontal and vertical are two
   offset vectors used to move the ray endpoint across the
   screen, starting from the lower left corner.
*/
class camera {
public:
    camera (vec3 lookfrom, vec3 lookat, vec3 vup,
            float vfov, float aspect, float aperture, float focus_dist) {
        lens_radius = aperture / 2;
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;
        
        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        lower_left_corner = origin - 
            focus_dist * (half_width*u + half_height*v + w);
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }
    ray get_ray(float s, float t,
                std::uniform_real_distribution<>& dis, std::mt19937& gen) {
        vec3 rd = lens_radius * random_in_unit_disk(dis, gen);
        vec3 offset = u * rd.x() + v * rd.y();
        vec3 orig_off = origin + offset;
        return ray(orig_off,
                   lower_left_corner + s*horizontal + t*vertical - orig_off);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif
