#include "hedron.h"



void genTriangle(int& ind , Vector3*vertices, Vector3*normals, Vector2*texcoords, Vector3 a, Vector3 b, Vector3 c) {
    Vector3 ab = Vector3Subtract(b, a);
    Vector3 ac = Vector3Subtract(c, a);
    Vector3 norm = Vector3CrossProduct(ab, ac);

    texcoords[ind] = Vector2{a.x,a.y};
    normals[ind] = norm;
    vertices[ind++] = a;
    texcoords[ind] = Vector2{b.x, b.y};
    normals[ind] = norm;
    vertices[ind++] = b,
    texcoords[ind] = Vector2{c.x, c.y};
    normals[ind] = norm;
    vertices[ind++] = c;
}



Mesh genMeshTetrahedron(){
    Mesh mesh = { };
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    int vertexCount = 12;

    Vector3* vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector3* normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector2* texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));

    float unit = sqrtf(8);
    float length = 0.5f * (1.0f / sqrtf(3.0f/ 8.0f)) / unit;

    Vector3 verts [] = { Vector3{length, length, length},
                         Vector3{length, -length, -length},
                         Vector3{-length, length, -length},
                         Vector3{-length, -length, length}
                       };

    int ind=0;
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[1], verts[2]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[3], verts[1]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[2], verts[3]);
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[3], verts[2]);

    int numfaces=4;
    int* triangles = (int*)RL_MALLOC(numfaces*3*sizeof(int));
    for(int i=0; i!= numfaces*3; i++){
        triangles[i] = i;
    }

    mesh  = genMesh(mesh, vertexCount, numfaces, vertices, texcoords, normals, triangles);

    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);


    rlLoadMesh(&mesh, false);

    return mesh;
}

Mesh genMeshOctahedron(){
    Mesh mesh = {};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    int vertexCount = 24;

    Vector3* vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector3* normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector2* texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));

    float unit = sqrtf(2);
    float length =0.5f* sqrtf(2) / unit;

    Vector3 verts [] = { Vector3{0, length, 0},
                         Vector3{-length, 0, 0},
                         Vector3{0, 0, -length},
                         Vector3{length, 0, 0},
                         Vector3{0, 0, length},
                         Vector3{0, -length, 0},
                       };

    int ind=0;
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[2], verts[1]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[3], verts[2]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[4], verts[3]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[1], verts[4]);

    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[1], verts[2]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[2], verts[3]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[3], verts[4]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[4], verts[1]);


    int numfaces=8;
    int* triangles = (int*)RL_MALLOC(numfaces*3*sizeof(int));
    for(int i=0; i!= numfaces*3; i++){
        triangles[i] = i;
    }

    mesh  = genMesh(mesh, vertexCount, numfaces, vertices, texcoords, normals, triangles);
    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);


    rlLoadMesh(&mesh, false);

    return mesh;
}

Mesh genMeshDodecahedron(){
    Mesh mesh = {};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    int vertexCount = 108;

    Vector3* vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector3* normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector2* texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));

    float gratio =(1+ sqrtf(5)) / 2.0f;
    float gratioinv = 1.0f / gratio;

    float unit = 2.0 * gratioinv;
    float length = 0.5f * (1.0f / (1.0f/4.0f*(sqrtf(15.0f) + sqrtf(3)))) / unit;


    Vector3 verts [] = {  Vector3{length, length, length},
                          Vector3{length, length, -length},
                          Vector3{length, -length, length},
                          Vector3{length, -length, -length},
                          Vector3{-length, length, length},
                          Vector3{-length, length, -length},
                          Vector3{-length, -length, length},
                          Vector3{-length, -length, -length},
                          //+-xz plane
                          Vector3{0, length*gratio, length*gratioinv},
                          Vector3{0, length*gratio, -length*gratioinv},
                          Vector3{0, -length*gratio, length*gratioinv},
                          Vector3{0, -length*gratio, -length*gratioinv},
                          //+-yz plane
                          Vector3{length*gratio, length*gratioinv, 0},
                          Vector3{length*gratio, -length*gratioinv, 0},
                          Vector3{-length*gratio, length*gratioinv, 0},
                          Vector3{-length*gratio, -length*gratioinv, 0},
                          //+-xy plane
                          Vector3{ length*gratioinv, 0, length*gratio},
                          Vector3{-length*gratioinv, 0, length*gratio},
                          Vector3{ length*gratioinv, 0,  -length*gratio},
                          Vector3{-length*gratioinv, 0, -length*gratio},
                       };

    int ind=0;

    // +xz
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[8], verts[4]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[9], verts[1]);

    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[9], verts[8]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[1], verts[9]);
    genTriangle(ind, vertices, normals, texcoords, verts[4], verts[8], verts[9]);
    genTriangle(ind, vertices, normals, texcoords, verts[4], verts[9], verts[5]);

    // -xz
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[6], verts[10]);
    genTriangle(ind, vertices, normals, texcoords, verts[7], verts[3], verts[11]);

    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[10], verts[11]);
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[11], verts[3]);
    genTriangle(ind, vertices, normals, texcoords, verts[6], verts[11], verts[10]);
    genTriangle(ind, vertices, normals, texcoords, verts[6], verts[7], verts[11]);

    // +xy
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[16], verts[2]);
    genTriangle(ind, vertices, normals, texcoords, verts[4], verts[6], verts[17]);

    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[17], verts[16]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[4], verts[17]);
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[16], verts[17]);
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[17], verts[6]);

    // -xy
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[3], verts[18]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[19], verts[7]);

    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[18], verts[19]);
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[19], verts[5]);
    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[19], verts[18]);
    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[7], verts[19]);

    // +yz
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[12], verts[1]);
    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[13], verts[2]);

    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[13], verts[12]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[2], verts[13]);
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[12], verts[13]);
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[13], verts[3]);

    // -yz
    genTriangle(ind, vertices, normals, texcoords, verts[4], verts[5], verts[14]);
    genTriangle(ind, vertices, normals, texcoords, verts[7], verts[6], verts[15]);

    genTriangle(ind, vertices, normals, texcoords, verts[4], verts[14], verts[15]);
    genTriangle(ind, vertices, normals, texcoords, verts[4], verts[15], verts[6]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[15], verts[14]);
    genTriangle(ind, vertices, normals, texcoords, verts[5], verts[7], verts[15]);


    int numfaces=36;
    int* triangles = (int*)RL_MALLOC(numfaces*3*sizeof(int));
    for(int i=0; i!= numfaces*3; i++){
        triangles[i] = i;
    }

    mesh  = genMesh(mesh, vertexCount, numfaces, vertices, texcoords, normals, triangles);
    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);


    rlLoadMesh(&mesh, false);

    return mesh;
}

Mesh genMeshIcosahedron(){
    Mesh mesh = {};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    int vertexCount = 108;

    Vector3* vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector3* normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    Vector2* texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));

    float gratio =(1+ sqrtf(5)) / 2.0f;
    float unit = sqrtf(1.0 + (gratio-1.0f)*(gratio-1.0f) + gratio*gratio);
    float length = 0.5f * (1.0f / (sqrtf(10+ 2*sqrtf(5)) / 4)) /unit;


    Vector3 verts [] ={ Vector3{0, length, length*gratio},
                        Vector3{0, length, -length*gratio},
                         Vector3{0, -length, length*gratio},
                         Vector3{0, -length, -length*gratio},

                        Vector3{length, length*gratio, 0.0f},
                        Vector3{length, -length*gratio, 0.0f},
                         Vector3{-length, length*gratio, 0.0f},
                         Vector3{-length, -length*gratio, 0.0f},

                        Vector3{length*gratio, 0, length},
                        Vector3{-length*gratio, 0, length},
                         Vector3{length*gratio, 0, -length},
                         Vector3{-length*gratio, 0, -length},
    };

    int ind =0;
    //+ xz
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[6], verts[9]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[4], verts[6]);
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[8], verts[4]);

    genTriangle(ind, vertices, normals, texcoords, verts[9], verts[6], verts[11]);
    genTriangle(ind, vertices, normals, texcoords, verts[8], verts[10], verts[4]);

    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[11], verts[6]);
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[6], verts[4]);
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[4], verts[10]);
    //- xz
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[9], verts[7]);
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[7], verts[5]);
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[5], verts[8]);

    genTriangle(ind, vertices, normals, texcoords, verts[9], verts[11], verts[7]);
    genTriangle(ind, vertices, normals, texcoords, verts[8], verts[5], verts[10]);

    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[7], verts[11]);
    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[5], verts[7]);
    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[10], verts[5]);

    // +yz
    genTriangle(ind, vertices, normals, texcoords, verts[0], verts[9], verts[2]);
    genTriangle(ind, vertices, normals, texcoords, verts[2], verts[8], verts[0]);

    // -yz
    genTriangle(ind, vertices, normals, texcoords, verts[1], verts[3], verts[11]);
    genTriangle(ind, vertices, normals, texcoords, verts[3], verts[1], verts[10]);

    int numfaces=20;
    int* triangles = (int*)RL_MALLOC(numfaces*3*sizeof(int));
    for(int i=0; i!= numfaces*3; i++){
        triangles[i] = i;
    }


    mesh  = genMesh(mesh, ind, numfaces, vertices, texcoords, normals, triangles);
    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);


    rlLoadMesh(&mesh, false);

    return mesh;
}
