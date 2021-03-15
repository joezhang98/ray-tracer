#include <iostream>
#include "color.h"
#include "ray.h"
#include "vec3.h"

/* Solve for ray-sphere intersection using substitution of the ray
   equation for ray R into the equation of a sphere with center
   CENTER and radius RADIUS and applying the quadratic formula. */
bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.origin() - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius*radius;
    auto discriminant = b*b - 4*a*c;
    return (discriminant > 0);
}

/* Given a ray R, determines the color that would be observed at a
   particular location on the screen, and returns the color as a 
   3-vector encoding RGB values. */
color ray_color(const ray& r) {

    /* Color pixel red if ray hits a small sphere at image center. */
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);

    /* Linearly interpolate between blue and white for the
       background, starting with blue at the top. */
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    /* Set screen size. */
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    /* Set camera parameters. */
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin 
        - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    /* Write pixels out in rows from left to right, starting at the
       top row and ending at the bottom row. */
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);

            ray r(origin,
                  lower_left_corner + u*horizontal + v*vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}