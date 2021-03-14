#ifndef SCENESH
#define SCENESH
#include <random>
#include "hittable-list.h"
#include "material-types.h"
#include "sphere.h"

/* Scene with lots of random spheres. */
hittable *random_scene(std::uniform_real_distribution<>& dis,
                       std::mt19937& gen) {
    int n = 500;
    hittable **list = new hittable*[n+1];

    /* Surface sphere that acts as the ground. */
    list[0] = new sphere(vec3(0, -1000, 0), 1000,
                         new lambertian(vec3(0.5, 0.5, 0.5)));
    
    /* Small spheres of assorted types. */
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_material = dis(gen);
            vec3 center(a + 0.9*dis(gen), 0.2, b + 0.9*dis(gen));
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_material < 0.8) {
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(
                                           dis(gen)*dis(gen),
                                           dis(gen)*dis(gen),
                                           dis(gen)*dis(gen))));
                }
                else if (choose_material < 0.95) {
                    list[i++] = new sphere(center, 0.2, new metal(vec3(
                                           0.5 * (1 + dis(gen)),
                                           0.5 * (1 + dis(gen)),
                                           0.5 * (1 + dis(gen))),
                                           0.5 * dis(gen)));
                }
                else {
                    list[i++] = new sphere(center, 0.2,
                                           new dielectric(1.5));
                }
            }
        }
    }

    /* Three large spheres (dielectric (glass), diffuse, and metal). */
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0,
                           new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0,
                           new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hittable_list(list, i);
}

#endif
