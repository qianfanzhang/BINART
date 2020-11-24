#ifndef MY_RAY_H
#define MY_RAY_H

#include "vecmath.h"
#include <iostream>

// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray {
public:
    Ray() = delete;
    Ray(const Vector3f &orig, const Vector3f &dir) {
        origin = orig;
        direction = dir.normalized();
    }

    Ray(const Ray &r) {
        origin = r.origin;
        direction = r.direction;
    }

    const Vector3f &getOrigin() const {
        return origin;
    }

    const Vector3f &getDirection() const {
        return direction;
    }

    Vector3f pointAtParameter(float t) const {
        return origin + direction * t;
    }

private:
    Vector3f origin;
    Vector3f direction;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}

#endif