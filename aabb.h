#ifndef AABB_H
#define AABB_H

#include "util.h"

/*
   An axis-aligned bounding box (AABB) class to represent bounding
   boxes over objects in a scene.

   A bounding box is encoded by two points that can be thought of as
   points opposite of each other in a rectangular volume.
*/
class aabb {
public:
    aabb() {}
    aabb(const point3& a, const point3& b) { minimum = a; maximum = b; }

    point3 min() const { return minimum; }
    point3 max() const { return maximum; }

    /* An optimized method for determinining intersection of ray R with
    an axis-aligned bounding box on the interval [T_MIN, T_MAX). */
    bool hit(const ray& r, double t_min, double t_max) const {
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (minimum[a] - r.origin()[a]) * invD;
            auto t1 = (maximum[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }

        return true;
    }

    point3 minimum;
    point3 maximum;
};

/* Returns the bounding box which encloses both BOX0 and BOX1. */
aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));
    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}

#endif
