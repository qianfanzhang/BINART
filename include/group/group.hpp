#ifndef MY_GROUP_H
#define MY_GROUP_H

#include "hit.hpp"
#include "light.hpp"
#include "medium.hpp"
#include "material.hpp"
#include "object/object3d.hpp"
#include "random.hpp"
#include "ray.hpp"
#include "vecmath.h"
#include <iostream>
#include <vector>

class Group {
public:
    virtual ~Group() = default;

    virtual void add(Object3D *obj) {
        const std::vector<Object3D *> basic_objects = obj->getBasicObjects();
        objects.insert(objects.end(), basic_objects.begin(), basic_objects.end());
    }

    void add(Light *li) {
        lights.push_back(li);
    }

    bool intersect(const Ray &r, Hit &h) {
        return intersect(r, h, Hit::T_MIN);
    }

    Vector3f sampleAllLights(const Ray &ray, const Hit &hit, Material *material, Medium *medium, RandomGenerator &gen) {
        Vector3f L = Vector3f::ZERO;
        if (material != nullptr && material->isDeltaBSDF())
            return L;

        Vector3f point = ray.pointAtParameter(hit.t);

        for (Light *li : lights) {
            Vector3f dir;
            float li_t;
            Vector3f intensity = li->sampleRay(point, dir, li_t, gen);
            if (intensity.length() < 1e-3)
                continue;
            Hit tmp_hit;
            tmp_hit.t = li_t;
            if (!intersect(Ray(point, dir), tmp_hit)) {
                // FIXME: unhandleled medium interaction
                if (material != nullptr) {
                    float f = material->BSDF(dir, -ray.direction, hit.normal) * std::abs(Vector3f::dot(dir, hit.normal));
                    L += intensity * f;
                } else {
                    assert(medium != nullptr);
                    float f = medium->pdf(dir, -ray.direction);
                    L += intensity * f;
                }
            }
        }

        return L;
    }

    virtual void build() {}

    virtual void debug() = 0;

    std::vector<Light *> lights;

protected:
    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;

    std::vector<Object3D *> objects;
};

#endif