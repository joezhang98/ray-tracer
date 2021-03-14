#ifndef CAMERAH
#define CAMERAH
#include "ray.h"

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
    camera (float vfov, float aspect) {
        float theta = vfov * M_PI / 180;
        float half_height = tan(theta/2);
        float half_width = aspect * half_height;

        lower_left_corner = vec3(-half_width, -half_height, -1.0);
        horizontal = vec3(2*half_width, 0.0, 0.0);
        vertical = vec3(0.0, 2*half_height, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }
    ray get_ray(float u, float v) {
        return ray(origin,
                   lower_left_corner + u*horizontal + v*vertical - origin);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif
