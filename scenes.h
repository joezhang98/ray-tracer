#ifndef SCENES_H
#define SCENES_H

#include "bvh.h"
#include "hittable-list.h"
#include "material.h"
#include "moving-sphere.h"
#include "sphere.h"

/* Scene with lots of random spheres (case 0 in main). */
hittable_list random_scene() {
    hittable_list world;

    /* Sphere that acts as the ground. */
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0, -1000, 0),
                                  1000, ground_material));

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

#endif
