#include "mazemesh.h"

void genBackWall(int &ind, Vector3* normals, Vector2* texcoords, Vector3*vertices , float x, float z, Rectangle texCoord){
    static constexpr Vector3 norm = Vector3{0.0f, 0.0f, 1.0f};

    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ float(x), -0.5f, float(z) };
    normals[ind] =  norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ x+1.0f, -0.5f, float(z) };
    normals[ind] =  norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y };
    vertices[ind++] = Vector3{ x+1.0f , +0.5f, float(z)};
    normals[ind] =  norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y };
    vertices[ind++] = Vector3{ float(x), +0.5f, float(z) };
}

void genFrontWall(int &ind, Vector3* normals, Vector2* texcoords, Vector3*vertices , float x,float z, Rectangle texCoord){
    static constexpr Vector3 norm = Vector3{0.0f, 0.0f, -1.0f};

    normals[ind] =  norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y };
    vertices[ind++] = Vector3{ x+1.0f , +0.5f, float(z)};
    normals[ind] =  norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ x+1.0f, -0.5f, float(z) };
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ float(x), -0.5f, float(z) };
    normals[ind] =  norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y };
    vertices[ind++] = Vector3{ float(x), +0.5f, float(z) };
}

void genrightWall(int &ind, Vector3* normals, Vector2* texcoords, Vector3*vertices , float x,float z, Rectangle texCoord){
    static constexpr Vector3 norm = Vector3{-1.0f, 0.0f, 0.0f};

    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ float(x), -0.5f, float(z) };
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ float(x), -0.5f, z +1.0f};
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y };
    vertices[ind++] = Vector3{ float(x), +0.5f, z +1.0f};
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y };
    vertices[ind++] = Vector3{ float(x), +0.5f, float(z) };
}

void genLeftWall(int &ind, Vector3* normals, Vector2* texcoords, Vector3*vertices , float x,float z, Rectangle texCoord){
    static constexpr Vector3 norm = Vector3{1.0f, 0.0f, 0.0f};

    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y };
    vertices[ind++] = Vector3{ float(x), +0.5f, z +1.0f};
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ float(x), -0.5f, z +1.0f};
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y+texCoord.height };
    vertices[ind++] = Vector3{ float(x), -0.5f, float(z) };
    normals[ind] = norm;
    texcoords[ind] = Vector2{ texCoord.x+texCoord.width, texCoord.y };
    vertices[ind++] = Vector3{ float(x), +0.5f, float(z) };
}



Mesh genMeshWall(Texture2D atras, const MazeMap& map, Rectangle texCoord, Rectangle picCoord){

    texCoord.x /= atras.width;
    texCoord.y /= atras.height;
    texCoord.width /= atras.width;
    texCoord.height /= atras.height;

    picCoord.x /= atras.width;
    picCoord.y /= atras.height;
    picCoord.width /= atras.width;
    picCoord.height /= atras.height;

    Mesh mesh ={};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    int vertexCount = map.x()*map.z()*16 + map.x()*8 + map.z()*8; // max:

    Vector3 *vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    // Normals definition
    Vector3 *normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));

    Vector2 *texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));
    int ind = 0 ;
    for (int z = 0; z < map.z(); z++)
    {
        for (int x = 0; x < map.x(); x++)
        {
            float xp = x;
            float zp = z;
            if(map[z][x].back) {
                int r = (random()% 1000 > 4);
                genBackWall(ind, normals, texcoords, vertices, xp, zp, r ? texCoord : picCoord);
                 r = (random()% 1000 > 4);
                genFrontWall(ind, normals, texcoords, vertices, xp, zp, r ? texCoord : picCoord);
            }

            if(map[z][x].right) {
                int r = (random()% 1000 > 4);
                genrightWall(ind, normals, texcoords, vertices, xp, zp, r ? texCoord : picCoord);
                r = (random()% 1000 > 4);
                genLeftWall(ind, normals, texcoords, vertices, xp, zp, r ? texCoord : picCoord);
            }
        }
    }

    for (int x = 0; x < map.x(); x++)
    {
        int r = (random()% 1000 > 4);
        float xp = x;
    //back
        genBackWall(ind, normals, texcoords, vertices, xp, 0.0f, r ? texCoord : picCoord);
    //front
        r = (random()% 1000 > 4);
        genFrontWall(ind, normals, texcoords, vertices, xp, map.z(), r ? texCoord : picCoord);
    }

    for (int z = 0; z < map.z(); z++)
    {
        int r = (random()% 1000 > 4);
        float zp = z;
    //right
        genLeftWall(ind, normals, texcoords, vertices, 0, zp, r ? texCoord : picCoord);
    //left
        r = (random()% 1000 > 4);
        genrightWall(ind, normals, texcoords, vertices, map.x(), zp, r ? texCoord : picCoord);
    }

    vertexCount = ind;

    int numFaces = vertexCount/4;
    int *triangles = (int *)RL_MALLOC(numFaces*6*sizeof(int));
    int t = 0;
    for (int face = 0; face < numFaces; face++)
    {
        int in = face*4;
        triangles[t++] = in + 1;
        triangles[t++] = in + 3;
        triangles[t++] = in;

        triangles[t++] = in + 2;
        triangles[t++] = in + 3;
        triangles[t++] = in + 1;
    }


    mesh = genMesh(mesh, vertexCount, numFaces*2, vertices, texcoords, normals, triangles);

    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);

    rlLoadMesh(&mesh, false);

    return mesh;
}

Mesh genMeshFloor(Texture2D atras, int resX, int resZ, Rectangle down, Rectangle up)
{
    up.x /= atras.width;
    up.y /= atras.height;
    up.width /= atras.width;
    up.height /= atras.height;

    down.x /= atras.width;
    down.y /= atras.height;
    down.width /= atras.width;
    down.height /= atras.height;

    Mesh mesh = {};
    mesh.vboId = (unsigned int *)RL_CALLOC(MAX_MESH_VBO, sizeof(unsigned int));

    int width = resX;
    int length = resZ;

    int vertexCount = width*length*4*2;

    Vector3 *vertices = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {
            float xp = x;
            float zp = z;
            int ind = (x + z * width)*4 ;
            vertices[ind] = Vector3{ float(xp), -0.5f, float(zp) };
            vertices[ind+1] = Vector3{ float(xp), -0.5f, zp+1.0f };
            vertices[ind+2] = Vector3{ xp+1.0f,-0.5f, zp+1.0f };
            vertices[ind+3] = Vector3{ xp+1.0f, -0.5f, float(zp) };
        }
    }

    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {
            float xp = x;
            float zp = z;
            int ind = (x + z* width)*4 + vertexCount/2;
            vertices[ind] = Vector3{ float(xp), 0.5f, float(zp) };
            vertices[ind+1] = Vector3{ float(xp), 0.5f, zp+1.0f};
            vertices[ind+2] = Vector3{ xp+1.0f, 0.5f, zp+1.0f };
            vertices[ind+3] = Vector3{ xp+1.0f, 0.5f, float(zp) };
        }

    }

    // Normals definition
    Vector3 *normals = (Vector3 *)RL_MALLOC(vertexCount*sizeof(Vector3));
    for (int n = 0; n < vertexCount/2; n++) normals[n] = Vector3{ 0.0f, -1.0f, 0.0f };   // Vector3.up;
    for (int n = vertexCount/2; n < vertexCount; n++) normals[n] = Vector3{ 0.0f, 1.0f, 0.0f };   // Vector3.down;

    // TexCoords definition
    Vector2 *texcoords = (Vector2 *)RL_MALLOC(vertexCount*sizeof(Vector2));
    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {
            int ind = (x + z* width)*4 ;

            texcoords[ind] = Vector2{ up.x, up.y };
            texcoords[ind+1] = Vector2{ up.x, up.y+up.height };
            texcoords[ind+2] = Vector2{ up.x+up.width, up.y+up.height };
            texcoords[ind+3] = Vector2{ up.x+up.width, up.y };
        }
    }

    for (int z = 0; z < length; z++)
    {
        for (int x = 0; x < width; x++)
        {
            int ind = (x + z* width)*4 + vertexCount/2;

            texcoords[ind] = Vector2{ down.x, down.y };
            texcoords[ind+1] = Vector2{ down.x, down.y+down.height };
            texcoords[ind+2] = Vector2{ down.x+ down.width, down.y+down.height };
            texcoords[ind+3] = Vector2{ down.x+down.width, down.y };
        }

    }

    int numFaces = 2*width*length;
    int *triangles = (int *)RL_MALLOC(numFaces*6*sizeof(int));
    int t = 0;
    for (int face = 0; face < numFaces/2; face++)
    {
        int ind = face*4;
        triangles[t++] = ind + 1;
        triangles[t++] = ind + 3;
        triangles[t++] = ind;

        triangles[t++] = ind + 2;
        triangles[t++] = ind + 3;
        triangles[t++] = ind +1;
    }
    for (int face = numFaces/2; face < numFaces; face++)
    {
        int ind = face*4;
        triangles[t++] = ind ;
        triangles[t++] = ind + 3;
        triangles[t++] = ind + 1;

        triangles[t++] = ind + 1;
        triangles[t++] = ind + 3;
        triangles[t++] = ind + 2;
    }

    mesh = genMesh(mesh, vertexCount, numFaces*2, vertices, texcoords, normals, triangles);

    RL_FREE(vertices);
    RL_FREE(normals);
    RL_FREE(texcoords);
    RL_FREE(triangles);

    rlLoadMesh(&mesh, false);

    return mesh;
}

