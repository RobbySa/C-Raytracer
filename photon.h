//
//  photon.h
//  Raytracer
//
//  Created by Roberto Sasso on 11/01/2021.
//

#ifndef photon_h
#define photon_h

#include "vector.h"
#include "color.h"

class Photon{
public:
    float intensity;
    Vector direction;
    
    // The photon needs a type to know what it's properties would be
    // 1 - Direct Illumination, 2 - Indirecrt Photon, 3 - Caustic Photon, 4 - Shadow Photon
    int typeOfPhoton;
    
    Color photonColor;
    
    Photon(float i, Vector dir, int type, Color pC) {
        intensity = i;
        direction = dir;
        typeOfPhoton = type;
        
        photonColor = pC;
    }
};

#endif /* photon_h */
