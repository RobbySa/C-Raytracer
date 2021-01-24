//
// Created by Roberto Sasso on 18/11/2020.
//

#ifndef LAB3_LIGHT_H
#define LAB3_LIGHT_H

#include "directionalLight.h"
#include "pointLight.h"
#include "color.h"
#include "hit.h"

#include <vector>
using namespace std;

class Light {
public:
    vector<DirectionalLight> directionalLights;
    vector<PointLight> pointLights;
    float ambientIntensity;
    Color lightColor;

    Light(float aI, Color lC);

    void addDirectionalLight(DirectionalLight light);
    void addPointLight(PointLight light);
    Color ambientLighting(Hit hit);
    Color diffuseAndSpecularLighting(Hit hit, Vector lD, Vector look);
};

#endif //LAB3_LIGHT_H
