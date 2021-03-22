#ifndef BOX_H
#define BOX_H

#include "aarect.h"
#include "hittable-list.h"
#include "util.h"

/*
   A box object represented as six rectangles.
*/
class box : public hittable {
public:
    box() {}
    box(const point3& p0, const point3& p1, shared_ptr<material> ptr);

    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override {
        output_box = aabb(box_min, box_max);
        return true;
    }

public:
    point3 box_min;       /* One corner of the box. */
    point3 box_max;       /* Opposite corner (diagonal through center). */
    hittable_list sides;  /* List of rectangles making up faces of box. */
};

/* Constructs the box from two points P0 and P1 by creating six
   rectangle objects for the six faces of the box. */
box::box(const point3& p0, const point3& p1, shared_ptr<material> ptr) {
    box_min = p0;
    box_max = p1;

    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(),
                                   p1.z(), ptr));
    sides.add(make_shared<xy_rect>(p0.x(), p1.x(), p0.y(), p1.y(),
                                   p0.z(), ptr));

    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(),
                                   p1.y(), ptr));
    sides.add(make_shared<xz_rect>(p0.x(), p1.x(), p0.z(), p1.z(),
                                   p0.y(), ptr));

    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(), 
                                   p1.x(), ptr));
    sides.add(make_shared<yz_rect>(p0.y(), p1.y(), p0.z(), p1.z(),
                                   p0.x(), ptr));
}

/* Compute ray-box intersection as ray-rectangle intersection with
   the six rectangles that make up the faces of the box. */
bool box::hit(const ray& r, double t_min, double t_max,
              hit_record& rec) const {
    return sides.hit(r, t_min, t_max, rec);
}

#endif
