#pragma once

#include "./meshcommon.h"

typedef struct Hedron{
    Vector2 pos;
    enum{
        TETRA, OCTA, DODECA, ICOSA, NUM_TYPES,
    };
    int type;
    float angle;
}Hedron;

Mesh genMeshIcosahedron();
Mesh genMeshDodecahedron();
Mesh genMeshOctahedron();
Mesh genMeshTetrahedron();
