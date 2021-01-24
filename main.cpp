/**
 *
 * Final Submission
 *
 */

#include "framebuffer.h"
#include "polymesh.h"
#include "sphere.h"
#include "light.h"
#include "mesh.h"
#include "ray.h"
#include "hit.h"

#include <sys/time.h>

#include <vector>
#include <iostream>
using namespace std;

const int width = 1280;
const int height = 1280;

Ray cameraModel(int x, int y, Vertex eye)
{
    float xv = (-0.5f + ((float)x / width));
    float yv = (0.5f - ((float)y / height));
    Vector direction = Vector(xv, yv, 0.5);
    direction.normalise();

    Ray ray = Ray(eye, direction);
    return ray;
}

// Given a hit and a ray, it generates a reflection ray
Ray reflectionRay(Hit hit, Ray oldRay) {
    Vector reflection;
    hit.normal.reflection(oldRay.direction, reflection);
    reflection.normalise();
    
    Vertex correctedPosition = hit.position.add(reflection.multiply(0.0001).toVertex());
    
    return Ray(correctedPosition, reflection);
}

// Given a hit and a ray, it generates a reflection ray
Ray refractionRay(Hit hit, Ray oldRay) {
    Vector refraction;
    hit.normal.refraction(oldRay.direction, hit.what->material->refractionIndex, refraction);
    refraction.normalise();
    
    Vertex correctedPosition = hit.position.add(refraction.multiply(0.0001).toVertex());
    
    return Ray(correctedPosition, refraction);
}

//void updateReflectCoefficient(Hit hit, Ray ray) {
//    float cosI = hit.normal.dot(ray.direction);
//    float cosT = (float)sqrt((double) 1 - (1 / powf(hit.what->material->refractionIndex, 2)) * (1 - powf(cosI, 2)));
//
//    float rParallel = (hit.what->material->refractionIndex * cosI - cosT) / (hit.what->material->refractionIndex * cosI + cosT);
//    float rPerpendiculat = (cosI - hit.what->material->refractionIndex * cosT) / (cosI + hit.what->material->refractionIndex * cosT);
//
//    hit.what->material->reflect = 0.5 * (powf(rParallel, 2) + powf(rPerpendiculat, 2));
//}

void findHit(Ray ray, Object *objectInScene, Hit &hit)
{
    Object *obj = objectInScene;
    hit.flag = false;

    while (obj != 0) {
        Hit objHit;
        obj->intersection(ray, objHit);
        if (objHit.flag) {
            if (!hit.flag) {
                hit = objHit;
            } else if (hit.t > objHit.t) {
                hit = objHit;
            }
        }
        obj = obj->next;
    }
}

void raytrace(Ray ray, Object *objectInScene, Light *light, Color &color, int depth)
{
    Hit shadowHit;
    Hit primaryHit;

    // If hit is found compute ambient light
    findHit(ray, objectInScene, primaryHit);
    if (primaryHit.flag) {
        color.add(light->ambientLighting(primaryHit));

        // Loop through every light in the scene
        for (DirectionalLight l : light->directionalLights) {
            Vector view = Vector(-primaryHit.position.x, -primaryHit.position.y, -primaryHit.position.z);
            view.normalise();

            // Check if the surface is lit
            bool lit = true;
            if (l.direction.dot(primaryHit.normal) > 0) {
                lit = false;
            }
            // Check shadow
            if (lit) {
                Ray shadowRay = Ray(primaryHit.position, Vector(-l.direction.x, -l.direction.y, -l.direction.z));
                shadowRay.position = shadowRay.position.add(shadowRay.direction.multiply(0.0001).toVertex());

                // Check if the surface is covered by another one
                findHit(shadowRay, objectInScene, shadowHit);
                if (shadowHit.flag && shadowHit.t < 1000000000.0f) {
                    lit = false;
                }
            }
            // No shadow
            if (lit) {
                color.add(light->diffuseAndSpecularLighting(primaryHit, l.direction, view));
            }

            // Reflection and Refraction rays done by recursions
            if (depth != 0 && primaryHit.what->material->reflect > 0.0001f) {
                
                // Reflection
                Color reflectedColor;
                Ray reflectedRay = reflectionRay(primaryHit, ray);
                raytrace(reflectedRay, objectInScene, light, reflectedColor, (depth - 1));

                reflectedColor.scale(primaryHit.what->material->reflect);
                color.add(reflectedColor);
                
                // Refraction
                Color refractedColor;
                Ray refractedRay = refractionRay(primaryHit, ray);
                raytrace(refractedRay, objectInScene, light, refractedColor, (depth - 1));

                refractedColor.scale(1 - primaryHit.what->material->reflect);
                color.add(refractedColor);
            }
        }
    } else {
        color.r = 0.0;
        color.g = 0.0;
        color.b = 0.0;
    }
}

int main(int argc, char *argv[])
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    
    // Create a framebuffer and a transform matrix
    FrameBuffer *fb = new FrameBuffer(width, height);
    Transform *transform = new Transform(1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, -7.0f,
                                         0.0f, 1.0f, 0.0f, 15.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
    Transform *transformW1 = new Transform(2.0f, 0.0f, 0.0f, -8.25f,
                                         0.0f, 0.0f, 4.0f, -24.0f,
                                         0.0f, 4.0f, 0.0f, 8.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
    Transform *transformW2 = new Transform(-1.0f, 0.0f, 0.0f, 8.0f,
                                         0.0f, 0.0f, 4.0f, -24.0f,
                                         0.0f, 4.0f, 0.0f, 8.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
    Transform *transformW3 = new Transform(4.0f, 0.0f, 0.0f, -8.0f,
                                         0.0f, 0.0f, 4.0f, -24.0f,
                                         0.0f, 1.0f, 0.0f, 24.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
    Transform *transformF = new Transform(4.0f, 0.0f, 0.0f, -24.0f,
                                         0.0f, 0.0f, 1.0f, -8.0f,
                                         0.0f, 4.0f, 0.0f, 8.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);
    Transform *transformC = new Transform(4.0f, 0.0f, 0.0f, -24.0f,
                                         0.0f, 0.0f, 1.0f, 8.0f,
                                         0.0f, 4.0f, 0.0f, 8.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f);

    // POLYMESHES
    PolyMesh *pmT = new PolyMesh((char *)"teapot_smaller.ply", transform);
    PolyMesh *pmW1 = new PolyMesh((char *)"wallWindow.ply", transformW1);
    PolyMesh *pmW2 = new PolyMesh((char *)"wall.ply", transformW2);
    PolyMesh *pmW3 = new PolyMesh((char *)"backWall.ply", transformW3);
    PolyMesh *pmF = new PolyMesh((char *)"floor.ply", transformF);
    PolyMesh *pmC = new PolyMesh((char *)"floor.ply", transformC);
    // MATERIALS
    Material *generalMaterial = new Material(new float[3] {0.1f, 0.5f, 0.4f}, new float[3] {0.1f, 0.5f, 0.4f}, new float[3] {0.1f, 0.5f, 0.4f}, 100.0f, 0.0f);
    Material *mirrorMaterial = new Material(new float[3] {0.1f, 0.5f, 0.4f}, new float[3] {0.1f, 0.5f, 0.4f}, new float[3] {0.1f, 0.5f, 0.4f}, 100.0f, 0.9f);
    Material *wallMaterial = new Material(new float[3] {0.3f, 0.3f, 0.3f}, new float[3] {0.3f, 0.3f, 0.3f}, new float[3] {0.3f, 0.3f, 0.3f}, 100.0f, 0.0f);
    // COLORS
    Color *teapotColor = new Color(48.0 / 255.0, 135.0 / 255.0, 212.0 / 255.0, 1.0f);
    Color *wallColor = new Color(255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0, 1.0f);
    Color *floorColor = new Color(135.0 / 255.0, 212.0 / 255.0, 48.0 / 255.0, 1.0f);
    Color *ceilingColor = new Color(112.0 / 255.0, 128.0 / 255.0, 144 / 255.0, 1.0f);
    Color *mirrorColor = new Color(192 / 255.0, 192 / 255.0, 192 / 255.0, 0.2f);

    // OBJECTS IN THE SCENE
    Mesh *wallMesh1 = new Mesh(pmW1, wallMaterial, wallColor);
    Mesh *wallMesh2 = new Mesh(pmW2, wallMaterial, wallColor);
    Mesh *wallMesh3 = new Mesh(pmW3, wallMaterial, wallColor);
    Mesh *floorMesh =new Mesh(pmF, wallMaterial, floorColor);
    Mesh *ceilingMesh =new Mesh(pmC, wallMaterial, ceilingColor);
    Mesh *teapotMesh = new Mesh(pmT, generalMaterial, teapotColor);
    Sphere *mirrorSphere = new Sphere(Vertex(-4, 8, 20), 1.5f, mirrorMaterial, mirrorColor);
    // LINKING OBJECTS
    wallMesh1->next = wallMesh2;
    wallMesh2->next = wallMesh3;
    wallMesh3->next = floorMesh;
    floorMesh->next = ceilingMesh;
    ceilingMesh->next = mirrorSphere;
    mirrorSphere->next = teapotMesh;

    // Define the camera
    Vertex eyePosition = Vertex(0.0001,0 ,0);
    // Lights
    Light *lighting = new Light(0.01, Color());
    lighting->addDirectionalLight(DirectionalLight(Vector(1, -0.5, 0 ), 1));

    float progress = 0.0f;
    int log = 0;
    // Main loop
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            progress += 1.0;
            log++;
            if (log == 100) {
                log = 0;
                cout << progress/(width * height / 100) << "%" << endl;
            }
            Ray ray = cameraModel(i, j, eyePosition);

            Object *obj = wallMesh1;
            Color colorOfPixel;
            int depth = 5;

            raytrace(ray, obj, lighting, colorOfPixel, depth);

            fb->plotPixel(i, j, colorOfPixel.r, colorOfPixel.g, colorOfPixel.b);
            //fb->plotDepth(i, j, bestHit.t);
        }
    }

    // Output the framebuffer.
    fb->writeRGBFile((char *)"test2.ppm");

    struct timeval tc;
    gettimeofday(&tc, NULL);

    cout << tc.tv_sec - tp.tv_sec;
    cout << "s elapsed" << endl;
    
    return 0;
}
