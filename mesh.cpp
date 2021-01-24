//
// Created by Roberto Sasso on 18/11/2020.
//

#include "mesh.h"

#include <iostream>
using namespace std;

Mesh::Mesh(PolyMesh *me, Material *ma, Color *c)
{
    mesh = me;
    material = ma;
    color = c;

    // Create sphere that delimits the mesh, this reduces the amount of calculations needed in the raytracing
    Vertex centreOfMesh = Vertex(me->largestPoint.x - me->limitsOfTheMesh.x,
                                 me->largestPoint.y - me->limitsOfTheMesh.y,
                                 me->largestPoint.z - me->limitsOfTheMesh.z);
    boundsOfMesh = new Sphere(centreOfMesh, me->limitsOfTheMesh.absoluteVector(), ma, c);
}

void Mesh::intersection(Ray ray, Hit &hit)
{
    Hit triangleHit;
    hit.flag = false;

    // If the ray doesn't hit the bounds then it definitely doesn't hit the mesh
    Hit boundaryHit;
    boundsOfMesh->intersection(ray, boundaryHit);

    // If the boundaries have been hit then try to hit the mesh
    if (boundaryHit.flag) {
        float t = 1000000000.0;

        for (int i = 0; i < mesh->triangle_count; i++) {
            triangleIntersection(ray, triangleHit, i, t);

            if (triangleHit.flag) {
                if (!hit.flag) {
                    hit = triangleHit;
                } else if (triangleHit.t < hit.t) {
                    hit = triangleHit;
                }
            }
        }

        if (hit.flag) {
            if (hit.normal.dot(ray.direction) > 0.0) {
                hit.normal.negate();
            }
        }
    }
}

// calculation for each triangle
void Mesh::triangleIntersection(Ray ray, Hit &hit, int triangle, float &currentT)
{
    hit.flag = false;

    // Variables that will be needed later
    Vector ab, ac, n, q, h, s, positionEye;
    Vertex pointA, pointB, pointC;
    float a, f, u, v;

    positionEye = Vector(ray.position.x, ray.position.y, ray.position.z);
    pointA = mesh->vertex[mesh->triangle[triangle][0]];
    pointB = mesh->vertex[mesh->triangle[triangle][1]];
    pointC = mesh->vertex[mesh->triangle[triangle][2]];

    // Test if the ray intersect the plane the triangle in on
    ab.findVector(pointA, pointB);
    ac.findVector(pointA, pointC);
    ab.cross(ac, n);
    n.normalise();

    // If the plane has no intersection this if statement won't resolve
    if (fabs(mesh->faceNormal[triangle].dot(ray.direction)) < 0.0000000001) {
        return;
    }
    // If any of these return true then there is not intersection
    ray.direction.cross(ac, h);
    a = ab.dot(h);
    if (a > -0.00001f && a < 0.00001f) {
        return;
    }
    f = 1/a;
    s = positionEye.add(Vector(-pointA.x, -pointA.y, -pointA.z));
    u = f * s.dot(h);
    if (u < 0.0f || u > 1.0f) {
        return;
    }
    s.cross(ab,q);
    v = f * ray.direction.dot(q);
    if ((v < 0.0f) || ((u + v) > 1.0f)) {
        return;
    }

    // calculate t
    float t = f * ac.dot(q);
    // Don't calculate if it's negative or if it's smaller than the current hit
    if (t > 0.00001f && t < currentT) {
        Vertex intersectionPoint = Vertex(ray.position.x + (ray.direction.x * t), ray.position.y + (ray.direction.y * t), ray.position.z + (ray.direction.z * t));

        hit.t = t;
        hit.what = this;
        hit.flag = true;
        hit.position = intersectionPoint;
        hit.normal = mesh->faceNormal[triangle];
        hit.normal.normalise();

        currentT = t;
    }
}
