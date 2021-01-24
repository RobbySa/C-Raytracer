/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#pragma once

#include "vertex.h"
#include "vector.h"
#include "transform.h"

typedef int TriangleIndex[3];

class PolyMesh {
public:
    int vertex_count;
    int triangle_count;
    Vertex *vertex;
    Vector *faceNormal;
    Vector *vertexNormal;
    TriangleIndex *triangle;

    Vertex largestPoint;
    Vertex smallestPoint;
    Vector limitsOfTheMesh;

    void do_construct(char *file, Transform *transform);

    // For wall generation
    PolyMesh(char *file);
    PolyMesh(char *file, Transform *transform);

    void computeFacesNormals(int currentTriangle, Vector &normal);
    void computeVertexNormals();
};
