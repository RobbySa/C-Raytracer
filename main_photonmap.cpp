////
////  main_photonmap.cpp
////  Raytracer
////
////  Created by Roberto Sasso on 11/01/2021.
////
//
//#include "framebuffer.h"
//#include "polymesh.h"
//#include "photon.h"
//#include "sphere.h"
//#include "light.h"
//#include "mesh.h"
//#include "ray.h"
//#include "hit.h"
//
//#include <sys/time.h>
//
//#include <vector>
//#include <iostream>
//using namespace std;
//
//const int width = 1280;
//const int height = 1280;
//
//Ray cameraModel(int x, int y, Vertex eye)
//{
//    float xv = (-0.5f + ((float)x / width));
//    float yv = (0.5f - ((float)y / height));
//    Vector direction = Vector(xv, yv, 0.5);
//    direction.normalise();
//
//    Ray ray = Ray(eye, direction);
//    return ray;
//}
//
//// Given a hit and a ray, it generates a reflection ray
//Ray reflectionRay(Hit hit, Ray oldRay) {
//    Vector reflection;
//    hit.normal.reflection(oldRay.direction, reflection);
//    reflection.normalise();
//
//    Vertex correctedPosition = hit.position.add(reflection.multiply(0.0001).toVertex());
//
//    return Ray(correctedPosition, reflection);
//}
//
//// Given a hit and a ray, it generates a reflection ray
//Ray refractionRay(Hit hit, Ray oldRay) {
//    Vector refraction;
//    hit.normal.refraction(oldRay.direction, hit.what->material->refractionIndex, refraction);
//    refraction.normalise();
//
//    Vertex correctedPosition = hit.position.add(refraction.multiply(0.0001).toVertex());
//
//    return Ray(correctedPosition, refraction);
//}
//
//void findHit(Ray ray, Object *objectInScene, Hit &hit)
//{
//    Object *obj = objectInScene;
//    hit.flag = false;
//
//    while (obj != 0) {
//        Hit objHit;
//        obj->intersection(ray, objHit);
//        if (objHit.flag) {
//            if (!hit.flag) {
//                hit = objHit;
//            } else if (hit.t > objHit.t) {
//                hit = objHit;
//            }
//        }
//        obj = obj->next;
//    }
//}
//
//
///// Trace the path of the photon until a maximum of 5 reflections are reached
//void pathTrace(Photon photon, Object *objectInScene, PointLight light, int depth) {
//    if (depth == 0) {
//        return;
//    }
//
//    Hit primaryHit = Hit();
//    Ray ray = Ray(light.position, photon.direction);
//
//    findHit(ray, objectInScene, primaryHit);
//
//    // On a hit record the photon
//    if (primaryHit.flag) {
//
//    // If there was no further hit, the photon was lost
//    } else {
//        return;
//    }
//}
//
//void createPhotonMap(Object *objectInScene, Light *light) {
//    for (PointLight l : light->pointLights) {
//        float intensityOfEachPhoton = l.intesity / l.numberOfPhoton;
//        for (int i = 0; i < l.numberOfPhoton; i++) {
//            Vector direction = l.getRandomDirection();
//            Photon newPhoton = Photon(intensityOfEachPhoton, direction, 1, light->lightColor);
//            pathTrace(newPhoton, objectInScene, l, 5);
//        }
//    }
//}
//
