#pragma once

#include <cstdlib>
#include <memory>

typedef struct MapTile{
    bool right;
    bool back;
    bool item;
}MapTile;

class MazeMap{
public:
    void set(size_t x, size_t z);
    void generate();
    inline size_t x()const { return m_x; }
    inline size_t z()const { return m_z; }
    inline MapTile* operator[](size_t n) { return m_data.get() + m_z*n; }
    inline const MapTile* operator[](size_t n) const{ return m_data.get() + m_z*n; }

    bool* rightWall(int x, int z);
    bool rightIsWall(int x, int z);

    bool*backWall(int x, int z);
    bool backIsWall(int x, int z);

    bool* leftWall(int x, int z);
    bool leftIsWall(int x, int z);

    bool* frontWall(int x, int z);
    bool frontIsWall(int x, int z);

    bool diggableWall(int x, int z);
private:
    void generate(int x, int z);


    std::unique_ptr<MapTile[]> m_data;
    int m_x, m_z;
};
