#pragma once

#include "./meshcommon.h"
#include "./mazemap.h"

Mesh genMeshWall(Texture2D atras, const MazeMap& map, Rectangle texCoord, Rectangle picCoord);
Mesh genMeshFloor(Texture2D atras, int resX, int resZ, Rectangle down, Rectangle up);
