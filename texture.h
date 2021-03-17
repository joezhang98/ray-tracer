#ifndef TEXTURE_H
#define TEXTURE_H

#include "perlin.h"
#include "util.h"

/*
   A texture class to support textures on surfaces.
*/
class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

/*
   A solid color as a texture.
*/
class solid_color : public texture {
public:
    solid_color() {}
    solid_color(color c) : color_value(c) {}
    solid_color(double red, double green, double blue)
        : solid_color(color(red, green, blue)) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

/*
   A checkerboard texture using solid colors.
*/
class checker_texture : public texture {
public:
    checker_texture() {}
    checker_texture(shared_ptr<texture> _even, shared_ptr<texture> _odd)
        : even(_even), odd(_odd) {}
    checker_texture(color c1, color c2)
        : even(make_shared<solid_color>(c1)),
          odd(make_shared<solid_color>(c2)) {}
    
    virtual color value(double u, double v, const point3& p) const override {
        auto sines = sin(10*p.x()) * sin(10*p.y()) * sin(10*p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

public:
    shared_ptr<texture> odd;   /* Color for the odd squares. */
    shared_ptr<texture> even;  /* Color for the even squares. */
};

/*
   A Perlin noise texture.
*/
class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(double sc) : scale(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
        
        /* Select between different variations of Perlin noise. */
        int perlin_type = 2;

        switch(perlin_type) {

            /* Regular Perlin noise. */
            case 0:
                return color(1, 1, 1) * 0.5 * (noise.noise(scale * p) + 1.0);
            
            /* Perlin noise with turbulence. */
            case 1:
                return color(1, 1, 1) * noise.turb(scale * p);

            /* Perlin noise with phase (i.e., marble-like). */
            case 2:
                return color(1, 1, 1) * 0.5 
                    * (sin(scale*p.z() + 10*noise.turb(p)) + 1.0);
        }

        /* Default: regular Perlin noise. */
        return color(1, 1, 1) * 0.5 * (noise.noise(scale * p) + 1.0);
    }

public:
    perlin noise;  /* Perlin noise. */
    double scale;  /* Scale to adjust noise frequency. */
};

#endif
