/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include <cmath>
#include "vertex.h"

class Vector {
public:
    float x;
    float y;
    float z;

    Vector(float px, float py, float pz)
    {
        x = px;
        y = py;
        z = pz;
    }

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    void normalise()
    {
        float len = (float)sqrt((double)(x*x + y*y + z*z));
        x = x / len;
        y = y / len;
        z = z / len;
    }

    float dot(Vector &other)
    {
        return x*other.x + y*other.y + z*other.z;
    }

    void findVector(Vertex a, Vertex b)
    {
        x = b.x - a.x;
        y = b.y - a.y;
        z = b.z - a.z;
    }

    float absoluteVector()
    {
        return (float)sqrt((double) (x * x) + (y * y) + (z * z));
    }

    void reflection(Vector initial, Vector &reflect)
    {
        float d;

        d = dot(initial);
        d = d * 2.0f;

        reflect.x = initial.x - d * x;
        reflect.y = initial.y - d * y;
        reflect.z = initial.z - d * z;
    }
    
    void refraction(Vector initial, float indexOfRefraction, Vector &refract) {
        float cosI = dot(initial);
        float cosT = (float)sqrt((double) 1 - (1 / powf(indexOfRefraction, 2)) * (1 - powf(cosI, 2)));
        
        refract.x = (1 / indexOfRefraction) * initial.x - (cosT - (1 / indexOfRefraction) * cosI) * x;
        refract.y = (1 / indexOfRefraction) * initial.y - (cosT - (1 / indexOfRefraction) * cosI) * y;
        refract.z = (1 / indexOfRefraction) * initial.z - (cosT - (1 / indexOfRefraction) * cosI) * z;
    }

    void negate()
    {
        x = -x;
        y = -y;
        z = -z;
    }

    void cross(Vector &other, Vector &result)
    {
        result.x = y*other.z - z*other.y;
        result.y = z*other.x - x*other.z;
        result.z = x*other.y - y*other.x;
    }

    Vector multiply(float m)
    {
        return Vector(x * m, y * m, z * m);
    }

    Vector add(Vector v)
    {
        return Vector(x + v.x, y + v.y, z + v.z);
    }

    Vertex toVertex() {
        return Vertex(x, y, z);
    }
};
