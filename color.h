#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include "vec3.h"

/* Converts PIXEL_COLOR to RGB values and writes to OUT. */
void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.r()) << ' '
        << static_cast<int>(255.999 * pixel_color.g()) << ' '
        << static_cast<int>(255.999 * pixel_color.b()) << '\n';
}

#endif
