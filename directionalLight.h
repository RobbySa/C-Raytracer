//
// Created by Roberto Sasso on 18/11/2020.
//

#ifndef LAB3_DIRECTIONALLIGHT_H
#define LAB3_DIRECTIONALLIGHT_H

#include "vector.h"

class DirectionalLight {
public:
    Vector direction;
    float intesity;

    DirectionalLight()
    {
        direction = Vector(0, 0, -1);
        intesity = 1;
    }

    DirectionalLight(Vector dir, float i)
    {
        direction = dir;
        direction.normalise();
        intesity = (i < 0.0) ? 0.0 : i;
        intesity = (i > 1.0) ? 1.0 : i;
    }
};

#endif //LAB3_DIRECTIONALLIGHT_H
