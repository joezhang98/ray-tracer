#include <fstream>
#include <iostream>
#include <random>
#include "camera.h"
#include "float.h"
#include "hittable-list.h"
#include "material-types.h"
#include "scenes.h"
#include "sphere.h"

/* Given a ray R and a scene with objects WORLD, determines the color
   that would be observed at a particular location on the screen, and
   returns the color as a 3-vector encoding RGB values.

   DEPTH determines the extent of recursion when a ray hits an object
   and is reflected or refracted. */
vec3 color(const ray& r, hittable *world, int depth,
           std::uniform_real_distribution<>& dis, std::mt19937& gen) {
    hit_record rec;

    /* Use 0.001 for T_MIN to remove shadow acne. */
    if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation,
                                               scattered, dis, gen)) {
            return attenuation * color(scattered, world, depth+1, dis, gen);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    else {

        /* Linearly interpolate between white and blue for the
           background, starting with blue at the top. */
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main() {
    std::ofstream file("ray-tracer.ppm");

    /* Initialize a uniform real distribution on the interval [0, 1)
       and a random number generator with time-dependent seed. */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    /* Set screen size (NX, NY) and supersampling rate (NS). */
    int nx = 1200;
    int ny = 800;
    int ns = 10;
    file << "P3\n" << nx << " " << ny << "\n255\n";

    hittable *world = random_scene(dis, gen);

    /* Construct camera. */
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0, 1, 0),
               20, float(nx)/float(ny), aperture, dist_to_focus);

    /* Write pixels out in rows from left to right, starting at the
       top row and ending at the bottom row. */
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);

            /* Average NS random samples inside each pixel to remove
               jaggies and anti-aliase the output image. */ 
            for (int s = 0; s < ns; s++) {
                float u = float(i + dis(gen)) / float(nx);
                float v = float(j + dis(gen)) / float(ny);
                ray r = cam.get_ray(u, v, dis, gen);
                vec3 p = r.point_at_parameter(2.0);  /* Unused? */
                col += color(r, world, 0, dis, gen);
            }
            col /= float(ns);

            /* Adds gamma correction. */
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            
            /* Converts the color vector to RGB values for output. */
            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);
            
            file << ir << " " << ig << " " << ib << "\n";
        }
    }

    file.close();
}
