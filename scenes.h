#ifndef SCENES_H
#define SCENES_H

#include "bvh.h"
#include "hittable-list.h"
#include "material.h"
#include "moving-sphere.h"
#include "sphere.h"

/* Scene with lots of random spheres (case 0). */
hittable_list random_scene() {
    hittable_list world;

    /* Sphere that acts as the ground with checkerboard texture. */
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1),
                                                color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0),
                                  1000, make_shared<lambertian>(checker)));

    /* Small spheres of assorted types. */
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a+0.9*random_double(), 0.2, b+0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {

                    /* Diffuse material that moves. */
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(center, center2, 
                                                         0.0, 1.0, 0.2,
                                                         sphere_material));
                }
                else if (choose_mat < 0.95) {

                    /* Metal material. */
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2,
                                                  sphere_material));
                }
                else {

                    /* Dielectric material. */
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2,
                                                  sphere_material));
                }
            }
        }
    }

    /* Three large spheres (dielectric, diffuse, and metal). */
    auto material1 = make_shared<dielectric>(1.5);
    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
    world.add(make_shared<sphere>(point3(-4, -1, 0), 1.0, material2));
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
    /* Build BVH of scene. */
    return hittable_list(make_shared<bvh_node>(world, 0.0, 1.0));
}

/* Scene with two checkered spheres (case 1). */
hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1),
                                                color(0.9, 0.9, 0.9));                                    
    objects.add (make_shared<sphere>(point3(0, -10, 0), 10,
                                     make_shared<lambertian>(checker)));
    objects.add (make_shared<sphere>(point3(0, 10, 0), 10,
                                     make_shared<lambertian>(checker)));

    return objects;
}

/* Scene with two spheres with Perlin noise (case 2). */
hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0),
                                    1000, make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0),
                                    2, make_shared<lambertian>(pertext)));

    return objects;
}

/* Scene with one sphere with Earth map image texture (case 3). */
hittable_list earth() {
    auto earth_texture = make_shared<image_texture>("images/earthmap.jpeg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

    return hittable_list(globe);
}

/* Wheel of Fortune scene (case 4). */
hittable_list wheel_of_fortune() {
    hittable_list objects;

    /* Marble ground (using Perlin noise). */
    auto marble = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0),
                                    1000, make_shared<lambertian>(marble)));

    /* CS 248 sphere with two metal spheres on left/right. */
    auto cs248_texture = make_shared<image_texture>("images/cs248.png");
    auto cs248 = make_shared<lambertian>(cs248_texture);
    auto cs248_lr = make_shared<metal>(color(0.8, 0.2, 0.0), 0.3);

    objects.add(make_shared<sphere>(point3(-6.5, 7, 0), 0.8, cs248));
    objects.add(make_shared<sphere>(point3(-8.1, 7, 0), 0.8, cs248_lr));
    objects.add(make_shared<sphere>(point3(-4.9, 7, 0), 0.8, cs248_lr));

    /* Final Project sphere with two dielectrics on left/right. */
    auto fp_texture = make_shared<image_texture>("images/final-project.png");
    auto fp = make_shared<lambertian>(fp_texture);
    auto fp_lr = make_shared<dielectric>(1.5);

    objects.add(make_shared<sphere>(point3(6.5, 5.5, 0), 0.8, fp));
    objects.add(make_shared<sphere>(point3(4.9, 5.5, 0), 0.8, fp_lr));
    objects.add(make_shared<sphere>(point3(8.1, 5.5, 0), 0.8, fp_lr));

    /* Wheel of Fortune sphere. */
    auto wof_texture = make_shared<image_texture>("images/wof.png");
    auto wof = make_shared<lambertian>(wof_texture);
    
    objects.add(make_shared<sphere>(point3(0, 6, 0), 2.5, wof));

    /* Build BVH of scene. */
    return hittable_list(make_shared<bvh_node>(objects, 0.0, 1.0));
}

#endif
