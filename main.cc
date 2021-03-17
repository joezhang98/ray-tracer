#include "util.h"
#include "scenes.h"
#include <iostream>
#include "camera.h"
#include "color.h"
#include "hittable-list.h"
#include "material.h"

/* Given a ray R and a list of objects WORLD, determines the color
   that would be observed at a particular location on the screen,
   returning the color as a 3-vector encoding RGB values.
   
   DEPTH determines the extent of recursion for a ray that reflects
   off an object surface. */
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    /* If recursion limit reached, return black (no light). */
    if (depth <= 0)
        return color(0, 0, 0);

    /* Simulate light rays reflecting off a surface. Use 0.001 for
       T_MIN to remove shadow acne. */
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth-1);
        
        return color(0, 0, 0);
    }

    /* Linearly interpolate between blue and white for the
       background, starting with blue at the top. */
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}

int main() {

    /* Set screen size and samples per pixel. */
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    
    /* Sets the maximum recursion depth for ray bounces. */
    const int max_depth = 50;

    /* Scene-specific world and camera parameters. */
    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    /* Select scene to render and assign parameters. */
    int scene_index = 0;

    switch(scene_index) {
        case 0:
            world = random_scene();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;
        default:
            std::cerr << "No scene selected.\n";
            return -1;
    }

    /* Make camera. */
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    camera cam(lookfrom, lookat, vup,
               vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    /* Write pixels out in rows from left to right, starting at the
       top row and ending at the bottom row. */
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);

            /* Avrerage SAMPLES_PER_PIXEL samples inside each pixel
               to remove jaggies in the output image. */
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
