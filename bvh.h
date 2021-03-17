#ifndef BVH_H
#define BVH_H

#include "hittable.h"
#include "hittable-list.h"
#include "util.h"

/*
   A bounding volume hierarchy (BVH) implemented as a tree of
   "hittable" BVH nodes.
*/
class bvh_node : public hittable {
public:
    bvh_node();
    bvh_node(const hittable_list& list, double time0, double time1)
        : bvh_node(list.objects, 0, list.objects.size(), time0, time1) {}
    bvh_node(const std::vector<shared_ptr<hittable>>& src_objects,
             size_t start, size_t end, double time0, double time1);
    
    virtual bool hit(const ray& r, double t_min, double t_max,
                     hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1,
                              aabb& output_box) const override;

public:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb box;
};

/* Stores the bounding box for the BVH node in OUTPUT_BOX. */
bool bvh_node::bounding_box(double time0, double time1,
                            aabb& output_box) const {
    output_box = box;
    return true;
}

/* Solve for intersection of the ray R with the tree of objects 
   rooted at BVH node, by recursively checking intersection of the
   ray with the children BVH nodes. T_MIN and T_MAX dictate the
   interval for a valid hit. If there is an intersection, store
   properties of the intersection in REC and return true. Otherwise,
   return false. */
bool bvh_node::hit(const ray& r, double t_min, double t_max,
                   hit_record& rec) const {
    if (!box.hit(r, t_min, t_max))
        return false;

    bool hit_left = left->hit(r, t_min, t_max, rec);
    bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);
    
    return hit_left || hit_right;                    
}

#endif
