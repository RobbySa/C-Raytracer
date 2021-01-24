//
//  pointLight.h
//  Raytracer
//
//  Created by Roberto Sasso on 10/01/2021.
//

#ifndef pointLight_h
#define pointLight_h

#include "vertex.h"

class PointLight {
public:
    Vertex position;
    float intesity;
    int numberOfPhoton;

    PointLight()
    {
        position = Vertex(0, 0, 0);
        intesity = 1;
        numberOfPhoton = 1000;
    }

    PointLight(Vertex pos, float i, int nP)
    {
        position = pos;
        intesity = (i < 0.0) ? 0.0 : i;
        intesity = (i > 1.0) ? 1.0 : i;
        numberOfPhoton = nP;
    }
    
    Vector getRandomDirection() {
        // get random between -1000 to 1000
        int randomX = (rand() % 2001) - 1000;
        float x = (float)randomX / 10000.0;
        int randomY = (rand() % 2001) - 1000;
        float y = (float)randomY / 10000.0;
        int randomZ = (rand() % 2001) - 1000;
        float z = (float)randomZ / 10000.0;
        
        return Vector(x,y,z);
    }
};

#endif /* pointLight_h */
