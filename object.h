/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#ifndef LAB3_OBJECT_H
#define LAB3_OBJECT_H

#include "ray.h"
#include "hit.h"
#include "material.h"
#include "color.h"

class Object {
public:
    Object *next;
    Material *material;
    Color *color;

    Object()
    {
        next = (Object *)0;
    }

    virtual void intersection(Ray ray, Hit &hit)
    {

    }
};

#endif //LAB3_OBJECT_H
