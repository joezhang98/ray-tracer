#ifndef SCENES_H
#define SCENES_H

#include "aarect.h"
#include "box.h"
#include "bvh.h"
#include "hittable-list.h"
#include "material.h"
#include "moving-sphere.h"
#include "sphere.h"

/* Scene with lots of random spheres (case 0). */
hittable_list random_scene() {
    hittable_list world;

    /* Sphere that acts as the ground with checkerboard texture. */
    auto checker = make_shared<checker_texture>(color(0, 0, 0),
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
    objects.add(make_shared<sphere>(point3(0, -10, 0), 10,
                                    make_shared<lambertian>(checker)));
    objects.add(make_shared<sphere>(point3(0, 10, 0), 10,
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
    // auto cs248_texture = make_shared<image_texture>("images/cs248.png");
    // auto cs248 = make_shared<lambertian>(cs248_texture);
    // auto cs248_lr = make_shared<metal>(color(0.8, 0.2, 0.0), 0.3);

    // objects.add(make_shared<sphere>(point3(-6.5, 7, 0), 0.8, cs248));
    // objects.add(make_shared<sphere>(point3(-8.1, 7, 0), 0.8, cs248_lr));
    // objects.add(make_shared<sphere>(point3(-4.9, 7, 0), 0.8, cs248_lr));

    /* Final Project sphere with two dielectrics on left/right. */
    // auto fp_texture = make_shared<image_texture>("images/final-project.png");
    // auto fp = make_shared<lambertian>(fp_texture);
    // auto fp_lr = make_shared<dielectric>(1.5);

    // objects.add(make_shared<sphere>(point3(6.5, 5.5, 0), 0.8, fp));
    // objects.add(make_shared<sphere>(point3(4.9, 5.5, 0), 0.8, fp_lr));
    // objects.add(make_shared<sphere>(point3(8.1, 5.5, 0), 0.8, fp_lr));

    /* Wheel of Fortune sphere. */
    auto wof_texture = make_shared<image_texture>("images/wof.png");
    auto wof = make_shared<lambertian>(wof_texture);
    
    objects.add(make_shared<sphere>(point3(0, 5.6, 0), 2.8, wof));

    /* Letter, number, and punctuation images. */
    auto let_A = make_shared<image_texture>("images/let_A.png");
    auto let_B = make_shared<image_texture>("images/let_B.png");
    auto let_C = make_shared<image_texture>("images/let_C.png");
    auto let_D = make_shared<image_texture>("images/let_D.png");
    auto let_E = make_shared<image_texture>("images/let_E.png");
    auto let_F = make_shared<image_texture>("images/let_F.png");
    auto let_G = make_shared<image_texture>("images/let_G.png");
    auto let_H = make_shared<image_texture>("images/let_H.png");
    auto let_I = make_shared<image_texture>("images/let_I.png");
    auto let_J = make_shared<image_texture>("images/let_J.png");
    auto let_K = make_shared<image_texture>("images/let_K.png");
    auto let_L = make_shared<image_texture>("images/let_L.png");
    auto let_M = make_shared<image_texture>("images/let_M.png");
    auto let_N = make_shared<image_texture>("images/let_N.png");
    auto let_O = make_shared<image_texture>("images/let_O.png");
    auto let_P = make_shared<image_texture>("images/let_P.png");
    auto let_Q = make_shared<image_texture>("images/let_Q.png");
    auto let_R = make_shared<image_texture>("images/let_R.png");
    auto let_S = make_shared<image_texture>("images/let_S.png");
    auto let_T = make_shared<image_texture>("images/let_T.png");
    auto let_U = make_shared<image_texture>("images/let_U.png");
    auto let_V = make_shared<image_texture>("images/let_V.png");
    auto let_W = make_shared<image_texture>("images/let_W.png");
    auto let_X = make_shared<image_texture>("images/let_X.png");
    auto let_Y = make_shared<image_texture>("images/let_Y.png");
    auto let_Z = make_shared<image_texture>("images/let_Z.png");

    auto num_2 = make_shared<image_texture>("images/num_2.png");

    auto apostrophe = 
        make_shared<image_texture>("images/punc_apostrophe.png");
    auto exclamation = 
        make_shared<image_texture>("images/punc_exclamation.png");

    /* Blank texture. */
    auto blank = make_shared<lambertian>(color(1.0, 1.0, 1.0));

    /* Letter, number, and punctuation textures. */
    auto text_A = make_shared<lambertian>(let_A);
    auto text_B = make_shared<lambertian>(let_B);
    auto text_C = make_shared<lambertian>(let_C);
    auto text_D = make_shared<lambertian>(let_D);
    auto text_E = make_shared<lambertian>(let_E);
    auto text_F = make_shared<lambertian>(let_F);
    auto text_G = make_shared<lambertian>(let_G);
    auto text_H = make_shared<lambertian>(let_H);
    auto text_I = make_shared<lambertian>(let_I);
    auto text_J = make_shared<lambertian>(let_J);
    auto text_K = make_shared<lambertian>(let_K);
    auto text_L = make_shared<lambertian>(let_L);
    auto text_M = make_shared<lambertian>(let_M);
    auto text_N = make_shared<lambertian>(let_N);
    auto text_O = make_shared<lambertian>(let_O);
    auto text_P = make_shared<lambertian>(let_P);
    auto text_Q = make_shared<lambertian>(let_Q);
    auto text_R = make_shared<lambertian>(let_R);
    auto text_S = make_shared<lambertian>(let_S);
    auto text_T = make_shared<lambertian>(let_T);
    auto text_U = make_shared<lambertian>(let_U);
    auto text_V = make_shared<lambertian>(let_V);
    auto text_W = make_shared<lambertian>(let_W);
    auto text_X = make_shared<lambertian>(let_X);
    auto text_Y = make_shared<lambertian>(let_Y);
    auto text_Z = make_shared<lambertian>(let_Z);

    auto text_2 = make_shared<lambertian>(num_2);

    auto text_apos = make_shared<lambertian>(apostrophe);
    auto text_excl = make_shared<lambertian>(exclamation);

    /* Spheres representing guesses remaining. */
    auto rainbow_r = make_shared<metal>(color(1.0, 0, 0), 0.3);
    auto rainbow_o = make_shared<metal>(color(1.0, 0.5, 0), 0.3);
    auto rainbow_y = make_shared<metal>(color(1.0, 1.0, 0), 0.3);
    auto rainbow_g = make_shared<metal>(color(0, 1.0, 0), 0.3);
    auto rainbow_b = make_shared<metal>(color(0, 0, 1.0), 0.3);
    auto rainbow_i = make_shared<metal>(color(0.3, 0, 0.5), 0.3);
    auto rainbow_v = make_shared<metal>(color(0.6, 0, 0.8), 0.3);

    objects.add(make_shared<sphere>(point3(-8.0, 5.0, 0), 0.5, rainbow_r));
    objects.add(make_shared<sphere>(point3(-6.0 - sqrt(3), 6.0, 0), 0.5, rainbow_o));
    objects.add(make_shared<sphere>(point3(-7.0, 5.0 + sqrt(3), 0), 0.5, rainbow_y));
    objects.add(make_shared<sphere>(point3(-6.0, 7.0, 0), 0.5, rainbow_g));
    objects.add(make_shared<sphere>(point3(-5.0, 5.0 + sqrt(3), 0), 0.5, rainbow_b));
    objects.add(make_shared<sphere>(point3(-6.0 + sqrt(3), 6.0, 0), 0.5, rainbow_i));
    objects.add(make_shared<sphere>(point3(-4.0, 5.0, 0), 0.5, rainbow_v));

    /* Spheres representing provided letters for final puzzle. */
    // objects.add(make_shared<sphere>(point3(4.0, 5.0, 0), 0.5, text_R));
    // objects.add(make_shared<sphere>(point3(5.0, 5.0, 0), 0.5, text_S));
    // objects.add(make_shared<sphere>(point3(6.0, 5.0, 0), 0.5, text_T));
    // objects.add(make_shared<sphere>(point3(7.0, 5.0, 0), 0.5, text_L));
    // objects.add(make_shared<sphere>(point3(8.0, 5.0, 0), 0.5, text_N));
    // objects.add(make_shared<sphere>(point3(9.0, 5.0, 0), 0.5, text_E));

    /* "HAPPY 22!" message. */
    objects.add(make_shared<sphere>(point3(4.0, 8.0, 0), 0.5, text_H));
    objects.add(make_shared<sphere>(point3(5.0, 8.0, 0), 0.5, text_A));
    objects.add(make_shared<sphere>(point3(6.0, 8.0, 0), 0.5, text_P));
    objects.add(make_shared<sphere>(point3(7.0, 8.0, 0), 0.5, text_P));
    objects.add(make_shared<sphere>(point3(8.0, 8.0, 0), 0.5, text_Y));

    objects.add(make_shared<sphere>(point3(5.0, 7.0, 0), 0.5, text_2));
    objects.add(make_shared<sphere>(point3(6.0, 7.0, 0), 0.5, text_2));
    objects.add(make_shared<sphere>(point3(7.0, 7.0, 0), 0.5, text_excl));


    /* "COMPUTER GRAPHICS IS COOL STUFF" text. */
    // objects.add(make_shared<sphere>(point3(-8.0, 1.5, 0), 0.5, text_C));
    // objects.add(make_shared<sphere>(point3(-7.0, 1.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(-6.0, 1.5, 0), 0.5, text_M));
    // objects.add(make_shared<sphere>(point3(-5.0, 1.5, 0), 0.5, text_P));
    // objects.add(make_shared<sphere>(point3(-4.0, 1.5, 0), 0.5, text_U));
    // objects.add(make_shared<sphere>(point3(-3.0, 1.5, 0), 0.5, text_T));
    // objects.add(make_shared<sphere>(point3(-2.0, 1.5, 0), 0.5, text_E));
    // objects.add(make_shared<sphere>(point3(-1.0, 1.5, 0), 0.5, text_R));

    // objects.add(make_shared<sphere>(point3(1.0, 1.5, 0), 0.5, text_G));
    // objects.add(make_shared<sphere>(point3(2.0, 1.5, 0), 0.5, text_R));
    // objects.add(make_shared<sphere>(point3(3.0, 1.5, 0), 0.5, text_A));
    // objects.add(make_shared<sphere>(point3(4.0, 1.5, 0), 0.5, text_P));
    // objects.add(make_shared<sphere>(point3(5.0, 1.5, 0), 0.5, text_H));
    // objects.add(make_shared<sphere>(point3(6.0, 1.5, 0), 0.5, text_I));
    // objects.add(make_shared<sphere>(point3(7.0, 1.5, 0), 0.5, text_C));
    // objects.add(make_shared<sphere>(point3(8.0, 1.5, 0), 0.5, text_S));

    // objects.add(make_shared<sphere>(point3(-6.0, 0.5, 0), 0.5, text_I));
    // objects.add(make_shared<sphere>(point3(-5.0, 0.5, 0), 0.5, text_S));

    // objects.add(make_shared<sphere>(point3(-3.0, 0.5, 0), 0.5, text_C));
    // objects.add(make_shared<sphere>(point3(-2.0, 0.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(-1.0, 0.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(-0.0, 0.5, 0), 0.5, text_L));

    // objects.add(make_shared<sphere>(point3(2.0, 0.5, 0), 0.5, text_S));
    // objects.add(make_shared<sphere>(point3(3.0, 0.5, 0), 0.5, text_T));
    // objects.add(make_shared<sphere>(point3(4.0, 0.5, 0), 0.5, text_U));
    // objects.add(make_shared<sphere>(point3(5.0, 0.5, 0), 0.5, text_F));
    // objects.add(make_shared<sphere>(point3(6.0, 0.5, 0), 0.5, text_F));

    /* "WORKING CODE ISN'T ENOUGH" text. */
    // objects.add(make_shared<sphere>(point3(-5.5, 1.5, 0), 0.5, text_W));
    // objects.add(make_shared<sphere>(point3(-4.5, 1.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(-3.5, 1.5, 0), 0.5, text_R));
    // objects.add(make_shared<sphere>(point3(-2.5, 1.5, 0), 0.5, text_K));
    // objects.add(make_shared<sphere>(point3(-1.5, 1.5, 0), 0.5, text_I));
    // objects.add(make_shared<sphere>(point3(-0.5, 1.5, 0), 0.5, text_N));
    // objects.add(make_shared<sphere>(point3(0.5, 1.5, 0), 0.5, text_G));

    // objects.add(make_shared<sphere>(point3(2.5, 1.5, 0), 0.5, text_C));
    // objects.add(make_shared<sphere>(point3(3.5, 1.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(4.5, 1.5, 0), 0.5, text_D));
    // objects.add(make_shared<sphere>(point3(5.5, 1.5, 0), 0.5, text_E));

    // objects.add(make_shared<sphere>(point3(-5.5, 0.5, 0), 0.5, text_I));
    // objects.add(make_shared<sphere>(point3(-4.5, 0.5, 0), 0.5, text_S));
    // objects.add(make_shared<sphere>(point3(-3.5, 0.5, 0), 0.5, text_N));
    // objects.add(make_shared<sphere>(point3(-2.5, 0.5, 0), 0.5, text_apos));
    // objects.add(make_shared<sphere>(point3(-1.5, 0.5, 0), 0.5, text_T));

    // objects.add(make_shared<sphere>(point3(0.5, 0.5, 0), 0.5, text_E));
    // objects.add(make_shared<sphere>(point3(1.5, 0.5, 0), 0.5, text_N));
    // objects.add(make_shared<sphere>(point3(2.5, 0.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(3.5, 0.5, 0), 0.5, text_U));
    // objects.add(make_shared<sphere>(point3(4.5, 0.5, 0), 0.5, text_G));
    // objects.add(make_shared<sphere>(point3(5.5, 0.5, 0), 0.5, text_H));

    /* "RUN LIKE YOU STOLE SOMETHING" text. */
    // objects.add(make_shared<sphere>(point3(-5.5, 1.5, 0), 0.5, text_R));
    // objects.add(make_shared<sphere>(point3(-4.5, 1.5, 0), 0.5, text_U));
    // objects.add(make_shared<sphere>(point3(-3.5, 1.5, 0), 0.5, text_N));

    // objects.add(make_shared<sphere>(point3(-1.5, 1.5, 0), 0.5, text_L));
    // objects.add(make_shared<sphere>(point3(-0.5, 1.5, 0), 0.5, text_I));
    // objects.add(make_shared<sphere>(point3(0.5, 1.5, 0), 0.5, text_K));
    // objects.add(make_shared<sphere>(point3(1.5, 1.5, 0), 0.5, text_E));

    // objects.add(make_shared<sphere>(point3(3.5, 1.5, 0), 0.5, text_Y));
    // objects.add(make_shared<sphere>(point3(4.5, 1.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(5.5, 1.5, 0), 0.5, text_U));

    // objects.add(make_shared<sphere>(point3(-7.0, 0.5, 0), 0.5, text_S));
    // objects.add(make_shared<sphere>(point3(-6.0, 0.5, 0), 0.5, text_T));
    // objects.add(make_shared<sphere>(point3(-5.0, 0.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(-4.0, 0.5, 0), 0.5, text_L));
    // objects.add(make_shared<sphere>(point3(-3.0, 0.5, 0), 0.5, text_E));

    // objects.add(make_shared<sphere>(point3(-1.0, 0.5, 0), 0.5, text_S));
    // objects.add(make_shared<sphere>(point3(0.0, 0.5, 0), 0.5, text_O));
    // objects.add(make_shared<sphere>(point3(1.0, 0.5, 0), 0.5, text_M));
    // objects.add(make_shared<sphere>(point3(2.0, 0.5, 0), 0.5, text_E));
    // objects.add(make_shared<sphere>(point3(3.0, 0.5, 0), 0.5, text_T));
    // objects.add(make_shared<sphere>(point3(4.0, 0.5, 0), 0.5, text_H));
    // objects.add(make_shared<sphere>(point3(5.0, 0.5, 0), 0.5, text_I));
    // objects.add(make_shared<sphere>(point3(6.0, 0.5, 0), 0.5, text_N));
    // objects.add(make_shared<sphere>(point3(7.0, 0.5, 0), 0.5, text_G));

    /* "HAPPINESS IS HAVING HOTPOT WITH YOU" text. */
    objects.add(make_shared<sphere>(point3(-9.0, 1.5, 0), 0.5, text_H));
    objects.add(make_shared<sphere>(point3(-8.0, 1.5, 0), 0.5, text_A));
    objects.add(make_shared<sphere>(point3(-7.0, 1.5, 0), 0.5, text_P));
    objects.add(make_shared<sphere>(point3(-6.0, 1.5, 0), 0.5, text_P));
    objects.add(make_shared<sphere>(point3(-5.0, 1.5, 0), 0.5, text_I));
    objects.add(make_shared<sphere>(point3(-4.0, 1.5, 0), 0.5, text_N));
    objects.add(make_shared<sphere>(point3(-3.0, 1.5, 0), 0.5, text_E));
    objects.add(make_shared<sphere>(point3(-2.0, 1.5, 0), 0.5, text_S));
    objects.add(make_shared<sphere>(point3(-1.0, 1.5, 0), 0.5, text_S));

    objects.add(make_shared<sphere>(point3(1.0, 1.5, 0), 0.5, text_I));
    objects.add(make_shared<sphere>(point3(2.0, 1.5, 0), 0.5, text_S));

    objects.add(make_shared<sphere>(point3(4.0, 1.5, 0), 0.5, text_H));
    objects.add(make_shared<sphere>(point3(5.0, 1.5, 0), 0.5, text_A));
    objects.add(make_shared<sphere>(point3(6.0, 1.5, 0), 0.5, text_V));
    objects.add(make_shared<sphere>(point3(7.0, 1.5, 0), 0.5, text_I));
    objects.add(make_shared<sphere>(point3(8.0, 1.5, 0), 0.5, text_N));
    objects.add(make_shared<sphere>(point3(9.0, 1.5, 0), 0.5, text_G));

    objects.add(make_shared<sphere>(point3(-7.0, 0.5, 0), 0.5, text_H));
    objects.add(make_shared<sphere>(point3(-6.0, 0.5, 0), 0.5, text_O));
    objects.add(make_shared<sphere>(point3(-5.0, 0.5, 0), 0.5, text_T));
    objects.add(make_shared<sphere>(point3(-4.0, 0.5, 0), 0.5, text_P));
    objects.add(make_shared<sphere>(point3(-3.0, 0.5, 0), 0.5, text_O));
    objects.add(make_shared<sphere>(point3(-2.0, 0.5, 0), 0.5, text_T));

    objects.add(make_shared<sphere>(point3(0.0, 0.5, 0), 0.5, text_W));
    objects.add(make_shared<sphere>(point3(1.0, 0.5, 0), 0.5, text_I));
    objects.add(make_shared<sphere>(point3(2.0, 0.5, 0), 0.5, text_T));
    objects.add(make_shared<sphere>(point3(3.0, 0.5, 0), 0.5, text_H));

    objects.add(make_shared<sphere>(point3(5.0, 0.5, 0), 0.5, text_Y));
    objects.add(make_shared<sphere>(point3(6.0, 0.5, 0), 0.5, text_O));
    objects.add(make_shared<sphere>(point3(7.0, 0.5, 0), 0.5, text_U));

    /* Build BVH of scene. */
    return hittable_list(make_shared<bvh_node>(objects, 0.0, 1.0));
}

/* Scene with object(s) as simple light(s) (case 5). */
hittable_list simple_light() {
    hittable_list objects;

    auto pertext = make_shared<noise_texture>(4);
    objects.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                    make_shared<lambertian>(pertext)));
    objects.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                    make_shared<lambertian>(pertext)));

    auto difflight = make_shared<diffuse_light>(color(4, 4, 4));
    auto difflight_dim = make_shared<diffuse_light>(color(1, 1, 1));
    objects.add(make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));
    objects.add(make_shared<sphere>(point3(0, 6, 0), 1.5, difflight_dim));

    return objects;
}

/* Scene showing a basic "Cornell Box" (case 6). */
hittable_list cornell_box() {
    hittable_list objects;

    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    /* Objects making up the room and ceiling light. */
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    /* Two boxes in the room, rotated about Y. */
    shared_ptr<hittable> box1 = make_shared<box>(point3(0, 0, 0),
                                                 point3(165, 330, 165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    shared_ptr<hittable> box2 = make_shared<box>(point3(0, 0, 0),
                                                 point3(165, 165, 165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130, 0, 65));
    objects.add(box2);

    return objects;
}

#endif
