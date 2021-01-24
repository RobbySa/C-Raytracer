//
// Created by Roberto Sasso on 18/11/2020.
//

#include "light.h"
#include "object.h"

Light::Light(float aI, Color lC) {
    ambientIntensity = (aI < 0.0) ? 0.0 : aI;
    ambientIntensity = (aI > 1.0) ? 1.0 : aI;
    
    lightColor = lC;
}

void Light::addDirectionalLight(DirectionalLight light)
{
    directionalLights.push_back(light);
}

void Light::addPointLight(PointLight light)
{
    pointLights.push_back(light);
}

Color Light::ambientLighting(Hit hit)
{
    Color resultingColor = Color();
    // Calculate basic colors first by using ambient
    resultingColor.r = hit.what->color->r * hit.what->material->ambientConstant[0] * hit.what->color->a;
    resultingColor.g = hit.what->color->g * hit.what->material->ambientConstant[1] * hit.what->color->a;
    resultingColor.b = hit.what->color->b * hit.what->material->ambientConstant[2] * hit.what->color->a;

    return resultingColor;
}

Color Light::diffuseAndSpecularLighting(Hit hit, Vector lD, Vector look)
{
    Color resultingColor = Color();
    Vector toLight = lD;
    toLight.negate();
    Vector toView = look;
    toView.negate();

    // Diffuse light
    float diffusion = hit.normal.dot(toLight);
    if (diffusion < 0.0f) {
        // light behind surface
        return resultingColor;
    } else {
        resultingColor.r += hit.what->material->diffuseConstant[0] * diffusion * hit.what->color->a;
        resultingColor.g += hit.what->material->diffuseConstant[1] * diffusion * hit.what->color->a;
        resultingColor.b += hit.what->material->diffuseConstant[2] * diffusion * hit.what->color->a;
    }

    // Specular light
    Vector reflection;
    hit.normal.reflection(toLight, reflection);
    reflection.normalise();

    float h = reflection.dot(toView);
    if (h > 0.0f) {
        float p = (float)pow(h, hit.what->material->specularN);

        resultingColor.r += hit.what->material->specularConstant[0] * p * hit.what->color->a;
        resultingColor.g += hit.what->material->specularConstant[1] * p * hit.what->color->a;
        resultingColor.b += hit.what->material->specularConstant[2] * p * hit.what->color->a;
    }

    return resultingColor;
}
