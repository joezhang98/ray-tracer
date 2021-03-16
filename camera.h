#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

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
    camera(double vfov, double aspect_ratio) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = 
            origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    }

    ray get_ray(double u, double v) const {
        return ray(origin,
                   lower_left_corner + u*horizontal + v*vertical - origin);
    }

private:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif
