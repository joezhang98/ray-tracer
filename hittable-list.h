#ifndef HITTABLE_LISTH
#define HITTABLE_LISTH
#include "hittable.h"

/*
   A class to represent a list of hittable objects.
*/
class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(hittable **l, int n) {list = l, list_size = n; }
    virtual bool hit(const ray& r, float t_min, float t_max,
                     hit_record& rec) const;
    hittable **list;
    int list_size;
};

/* Determines the closest point of intersection between a ray R and
   a list of objects, subject to the valid hit interval T_MIN to
   T_MAX. Information about the point of closest intersection (if
   any) is stored in REC. */
bool hittable_list::hit(const ray& r, float t_min, float t_max,
                        hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (int i = 0; i < list_size; i++) {
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

#endif
