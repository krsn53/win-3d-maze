#pragma once

#include <memory.h>
#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include<rlgl.h>

#define MAX_MESH_VBO 7

Mesh genMesh(Mesh mesh, int vertexCount, int numFaces, Vector3*vertices, Vector2*texcoords, Vector3* normals, int*triangles);
