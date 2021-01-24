/***************************************************************************
 *
 * krt - Kens Raytracer - Coursework Edition. (C) Copyright 1997-2019.
 *
 * Do what you like with this code as long as you retain this comment.
 */

#include <iostream>
#include <fstream>
#include <string>

#include "polymesh.h"

using namespace std;

PolyMesh::PolyMesh(char *file)
{
    Transform *transform = new Transform();

    this->do_construct(file, transform);
}

PolyMesh::PolyMesh(char *file, Transform *transform)
{
    this->do_construct(file, transform);
}

void PolyMesh::do_construct(char *file, Transform *transform)
{
    ifstream myFile;
    myFile.open(file);

    ofstream secondFile;
    secondFile.open("test.txt");

    string line;
    string line2;
    string line3;

    while (true) {
        myFile >> line;
        if (line.compare("element") == 0) {
            myFile >> line;
            if (line.compare("vertex") == 0) {
                myFile >> line;
                vertex_count = stoi(line);
            } else if (line.compare("face") == 0) {
                myFile >> line;
                triangle_count = stoi(line);
                break;
            }
        }
    }

    vertex = new Vertex[vertex_count];
    faceNormal = new Vector[triangle_count];
    vertexNormal = new Vector[vertex_count];
    triangle = new TriangleIndex[triangle_count];

    largestPoint = Vertex(0, 0, 0);
    smallestPoint = Vertex(0, 0, 0);

    // read to save all the vertices and faces
    int counterVertex = 0;
    int counterFace = 0;

    while (counterVertex != vertex_count) {
        myFile >> line;
        myFile >> line2;
        myFile >> line3;

        vertex[counterVertex].x = stof(line);
        vertex[counterVertex].y = stof(line2);
        vertex[counterVertex].z = stof(line3);
        counterVertex++;

        // Check if any position is the greatest or smallest yet
        largestPoint.x = (largestPoint.x < stof(line)) ? stof(line) : largestPoint.x;
        largestPoint.y = (largestPoint.y < stof(line2)) ? stof(line2) : largestPoint.y;
        largestPoint.z = (largestPoint.z < stof(line3)) ? stof(line3) : largestPoint.z;

        smallestPoint.x = (smallestPoint.x > stof(line)) ? stof(line) : smallestPoint.x;
        smallestPoint.y = (smallestPoint.y > stof(line2)) ? stof(line2) : smallestPoint.y;
        smallestPoint.z = (smallestPoint.z > stof(line3)) ? stof(line3) : smallestPoint.z;
    }
    while (counterFace != triangle_count) {
        myFile >> line;
        if (line.compare("3") == 0) {
            myFile >> line;
            myFile >> line2;
            myFile >> line3;

            triangle[counterFace][0] = stoi(line);
            triangle[counterFace][1] = stoi(line2);
            triangle[counterFace][2] = stoi(line3);
            counterFace++;
        }
    }

    for (int i = 0; i < vertex_count; i++) {
        transform->apply(vertex[i]);
    }

    transform->apply(smallestPoint);
    transform->apply(largestPoint);

    limitsOfTheMesh.findVector(smallestPoint, largestPoint);
    limitsOfTheMesh.x = limitsOfTheMesh.x/2;
    limitsOfTheMesh.y = limitsOfTheMesh.y/2;
    limitsOfTheMesh.z = limitsOfTheMesh.z/2;

    for (int i = 0; i < triangle_count; i++) {
        computeFacesNormals(i, faceNormal[i]);
    }
    computeVertexNormals();

    myFile.close();
    secondFile.close();
}

void PolyMesh::computeFacesNormals(int currentTriangle, Vector &normal)
{
    Vector ab, ac;
    ab.x = vertex[triangle[currentTriangle][1]].x - vertex[triangle[currentTriangle][0]].x;
    ab.y = vertex[triangle[currentTriangle][1]].y - vertex[triangle[currentTriangle][0]].y;
    ab.z = vertex[triangle[currentTriangle][1]].z - vertex[triangle[currentTriangle][0]].z;

    ab.normalise();

    ac.x = vertex[triangle[currentTriangle][2]].x - vertex[triangle[currentTriangle][0]].x;
    ac.y = vertex[triangle[currentTriangle][2]].y - vertex[triangle[currentTriangle][0]].y;
    ac.z = vertex[triangle[currentTriangle][2]].z - vertex[triangle[currentTriangle][0]].z;

    ac.normalise();

    ab.cross(ac, normal);
    normal.normalise();
}

void PolyMesh::computeVertexNormals()
{
    for (int i = 0; i < triangle_count; i++) {
        for (int j = 0; j < 3; j++) {
            vertexNormal[triangle[i][j]].add(faceNormal[i]);
        }
    }
    for (int i = 0; i < vertex_count; i += 1)
    {
        vertexNormal[i].normalise();
    }
}
