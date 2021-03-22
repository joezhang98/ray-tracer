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
color ray_color(const ray& r, const color& background,
                const hittable& world, int depth) {
    hit_record rec;

    /* If recursion limit reached, return black (no light). */
    if (depth <= 0)
        return color(0, 0, 0);

    /* If the ray doesn't hit anything, return background color. Use
       0.001 for T_MIN to remove shadow acne. */
    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    /* If no scattered ray, return just the emitted color. */
    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background,
                                             world, depth-1);
}

int main() {

    /* Set screen size (debugging: 400, production: 1600). */
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    /* Set samples per pixel (debugging: 10, production: 20). */
    int samples_per_pixel = 10;
    
    /* Sets the maximum recursion depth for ray bounces. */
    int max_depth = 50;

    /* Default world and camera parameters. */
    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    color background(0, 0, 0);

    /* Select scene to render and assign parameters. */
    int scene_index = 6;

    switch(scene_index) {

        /* Random scene with many assorted spheres. */
        case 0:
            world = random_scene();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;

        /* Scene with two checkered spheres. */
        case 1:
            world = two_spheres();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;

        /* Scene with two spheres with Perlin noise. */
        case 2:
            world = two_perlin_spheres();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;

        /* Scene with Earth image texture on sphere. */
        case 3:
            world = earth();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            break;

        /* Wheel of Fortune scene. */
        case 4: 
            world = wheel_of_fortune();
            background = color(0.7, 0.8, 1.0);
            lookfrom = point3(0, 0, 30);
            lookat = point3(0, 3.5, 0);
            vfov = 20.0;
            break;

        /* Scene with simple lights. */
        case 5:
            world = simple_light();
            samples_per_pixel = 400;
            background = color(0, 0, 0);
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
            break;

        /* Scene with an empty "Cornell box". */
        case 6:
            world = cornell_box();
            aspect_ratio = 1.0;
            image_width = 600;
            samples_per_pixel = 200;
            background = color(0, 0, 0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            break;

        /* Provided scene index does not match any created scene. */
        default:
            background = color(0, 0, 0);
            break;
    }

    /* Make camera and screen. */
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    camera cam(lookfrom, lookat, vup,
               vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);
    int image_height = static_cast<int>(image_width / aspect_ratio);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    /* Write pixels out in rows from left to right, starting at the
       top row and ending at the bottom row. */
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);

            /* Average SAMPLES_PER_PIXEL samples inside each pixel
               to remove jaggies in the output image. */
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}
