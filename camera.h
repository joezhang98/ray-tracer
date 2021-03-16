#ifndef CAMERA_H
#define CAMERA_H

#include "util.h"

/*
   A camera class to encapsulate a camera with arbitary viewpoint and
   adjustable field of view (fov).

   lower_left_corner denotes the coordinate of the lower left corner
   of the screen, and horizontal and vertical are two offset vectors
   used to move the ray endpoint across the screen, starting from the
   lower left corner.

   origin denotes the position of the camera, and the trio of vectors
   (u, v, w) form an orthonormal basis describing camera orientation.
*/
class camera {
public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, 
           double vfov, double aspect_ratio,
           double aperture, double focus_dist) {
        auto theta = degrees_to_radians(vfov);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        origin = lookfrom;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist*w;

        lens_radius = aperture / 2;
    }

    /* Returns the ray with origin at camera position (with defocus
       blur) and pointed at a location on the screen with horizontal
       and vertical coordinates determined by parameters S and T. */
    ray get_ray(double s, double t) const {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u*rd.x() + v*rd.y();
        vec3 orig_off = origin + offset;

        return ray(orig_off,
                   lower_left_corner + s*horizontal + t*vertical - orig_off);
    }

private:
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};

#endif
