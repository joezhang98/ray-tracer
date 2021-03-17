#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "hittable.h"

using std::shared_ptr;
using std::make_shared;

/*
   A class to represent a list of hittable objects.
*/
class hittable_list : public hittable {
public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() {objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override;

public:
    std::vector<shared_ptr<hittable>> objects;
};

/* Determines the closest point of intersection between a ray R and
   a list of objects, subject to the valid hit interval T_MIN to
   T_MAX. Information about the point of closest intersection (if
   any) is stored in REC. */
bool hittable_list::hit(const ray& r, double t_min, double t_max,
                        hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

/* Constructs a bounding box for the list of objects by iteratively
   constructing a surrounding box from the bounding box of the
   objects processed so far and the bounding box of the next object 
   in the list. Returns true unless the list is empty or the bounding
   box for any object cannot be constructed. */
bool hittable_list::bounding_box(double time0, double time1,
                                 aabb& output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    /* Construct the overall bounding box iteratively. */
    for (const auto& object : objects) {
        if (!object->bounding_box(time0, time1, temp_box))
            return false;

        output_box = first_box ? temp_box : surrounding_box(output_box,
                                                            temp_box);
        first_box = false;
    }

    return true;
}

#endif
