#include "mazemap.h"

void MazeMap::set(size_t x, size_t z){
    m_data.reset(new MapTile[x*z]);

    m_x = x;
    m_z = z;
}

void MazeMap::generate(int x, int z){
    int digged = 0xffff;
    while(digged != 0){
        int dir = random() & 0x3;
        switch (dir) {
        case 0:
            if((digged & 0xf000)&& diggableWall( x-1, z)) {
                auto ptr =rightWall(x,z);
                if(ptr){
                    *ptr = false;
                    generate(x-1, z);
                }
            }
            digged &= ~0xf000;
            break;
        case 1:
            if((digged & 0xf00)&& diggableWall(x, z-1)) {
                auto ptr =backWall(x,z);
                if(ptr){
                    *ptr = false;
                    generate(x, z-1);
                }
            }
            digged &= ~0xf00;

            break;
        case 2:
            if((digged & 0xf0)&& diggableWall( x+1, z)) {
                auto ptr =leftWall(x,z);
                if(ptr){
                    *ptr = false;
                    generate(x+1, z);
                }
            }
            digged &= ~0xf0;
            break;
        case 3:
            if((digged & 0xf)&& diggableWall(x, z+1)) {
                auto ptr =frontWall(x,z);
                if(ptr){
                    *ptr = false;
                    generate(x, z+1);
                }
            }
            digged &= ~0xf;
            break;
        }
    }
}

bool* MazeMap::rightWall(int x, int z){
    if(x <= 0 || z < 0) return nullptr;
    if(x >= m_x || z >= m_z) return nullptr;
    return &(*this)[z][x].right;
}

bool MazeMap::rightIsWall(int x, int z){
    auto ret =rightWall(x, z);
    return !ret || *ret;
}

bool*MazeMap::backWall(int x, int z){
    if(x < 0 || z <= 0) return nullptr;
    if(x >= m_x || z >= m_z) return nullptr;
    return &(*this)[z][x].back;
}

bool MazeMap::backIsWall(int x, int z){
    auto ret =backWall(x, z);
    return !ret || *ret;
}

bool* MazeMap::leftWall(int x, int z){
    if(x+1 < 0 || z < 0) return nullptr;
    if(x+1 >= m_x || z >= m_z) return nullptr;
    return &(*this)[z][x+1].right;
}

bool MazeMap::leftIsWall(int x, int z){
    auto ret =leftWall(x, z);
    return !ret || *ret;
}
bool* MazeMap::frontWall(int x, int z){
    if(x < 0 || z+1 < 0) return nullptr;
    if(x >= m_x || z+1 >= m_z) return nullptr;
    return &(*this)[z+1][x].back;
}

bool MazeMap::frontIsWall(int x, int z){
    auto ret = frontWall(x, z);
    return !ret || *ret;
}

bool MazeMap::diggableWall(int x, int z) {
    bool* ptr;
    return (!(ptr = rightWall(x,z)) || *ptr) &&
            (!(ptr = frontWall(x,z)) || *ptr) &&
            (!(ptr = leftWall(x,z)) || *ptr) &&
            (!(ptr = backWall(x,z)) || *ptr) ;
}

void MazeMap::generate(){
    if(m_data){
        for(int i= 0; i<m_z; i++){
            for(int j=0; j<m_x; j++){
                (*this)[i][j].right = j != 0;
                (*this)[i][j].back = i !=0;
                (*this)[i][j].item = false;
            }
        }

        generate(0, 0);
    }
}
