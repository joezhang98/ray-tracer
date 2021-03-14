#include <fstream>
#include <iostream>
#include <random>
#include "camera.h"
#include "float.h"
#include "hittable-list.h"
#include "material-types.h"
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
    int nx = 200;
    int ny = 100;
    int ns = 100;
    file << "P3\n" << nx << " " << ny << "\n255\n";

    /* Construct list of objects in the scene. */
    hittable *list[5];

    /* Small diffuse blue sphere. */
    list[0] = new sphere(vec3(0, 0, -1), 0.5,
                         new lambertian(vec3(0.1, 0.2, 0.5)));

    /* Large diffuse green sphere. */                     
    list[1] = new sphere(vec3(0, -100.5, -1), 100,
                         new lambertian(vec3(0.8, 0.8, 0.0)));

    /* Medium-fuzz metal. */
    list[2] = new sphere(vec3(1, 0, -1) ,0.5,
                         new metal(vec3(0.8, 0.6, 0.2), 0.5));
    
    /* Basic dielectric. */
    list[3] = new sphere(vec3(-1, 0, -1) ,0.5, new dielectric(1.5));

    /* Dielectric sphere with negative radius. */
    list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    
    hittable *world = new hittable_list(list, 5);
    camera cam;

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
                ray r = cam.get_ray(u, v);
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
