#include <fstream>
#include <iostream>
#include <random>
#include "camera.h"
#include "float.h"
#include "hittable-list.h"
#include "sphere.h"

vec3 random_in_unit_sphere(std::uniform_real_distribution<>& dis,
                           std::mt19937& gen) {
    vec3 p;
    do {
        p = 2.0 * vec3(dis(gen), dis(gen), dis(gen)) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    
    return p;
}

vec3 color(const ray& r, hittable *world,
           std::uniform_real_distribution<>& dis, std::mt19937& gen) {
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec)) {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere(dis, gen);
        return 0.5 * color(ray(rec.p, target - rec.p), world, dis, gen);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {

    /* Initialize a uniform real distribution on the interval [0, 1). */
    /* Initialize random number generator with time-dependent seed. */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::ofstream file("ray-tracer.ppm");

    int nx = 200;
    int ny = 100;
    int ns = 100;
    file << "P3\n" << nx << " " << ny << "\n255\n";

    hittable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hittable *world = new hittable_list(list, 2);

    camera cam;

    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + dis(gen)) / float(nx);
                float v = float(j + dis(gen)) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);  /* Unused? */
                col += color(r, world, dis, gen);
            }

            col /= float(ns);
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            
            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();
}
