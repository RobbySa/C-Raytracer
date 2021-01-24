//
// Created by Roberto Sasso on 18/11/2020.
//

#ifndef LAB3_MESH_H
#define LAB3_MESH_H

#include "polymesh.h"
#include "sphere.h"

class Mesh : public Object{
public:
    PolyMesh *mesh;
    Sphere *boundsOfMesh;

    Mesh(PolyMesh *me, Material *ma, Color *c);
    void intersection(Ray ray, Hit &hit);
    void triangleIntersection(Ray ray, Hit &hit, int triangle, float &currentT);
};

#endif //LAB3_MESH_H
