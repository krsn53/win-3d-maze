#pragma once

#include "./screen.h"
#include "./objects/hedron.h"
#include "./objects/mazemap.h"
#include <vector>



typedef enum Direction{
    DIRECTION_FRONT, DIRECTION_RIGHT, DIRECTION_BACK, DIRECTION_LEFT  , NUM_DIRECTIONS,
}Direction;



typedef struct Rat{
    Vector2 pos;
    int dir;
    int rotateFrame;
    float angle;
    float aVel;
    float  aTarget;
}Rat;

class Maze : public Screen
{
public:
    Maze();
    virtual void init();
    virtual Screen* update() ;
    virtual void draw() ;
    virtual void remove() ;

    static constexpr const int FPS = 60;
    static constexpr const float WIDTH = 1.5f;


    enum {
        START,
        MAIN,
        POSE,
        ROTATE,
        END,
    };

private:
    void start();
    void rotate();
    void pose();
    void goal();

    Camera3D m_camera;
    Texture2D m_atlas;
    // x=0 right, y=0 back
    MazeMap  m_map;
    Model m_floor, m_wall;
    Model m_tetra, m_octa, m_dodeca, m_icosa;
    Shader m_hedronShader, m_alphaclip;
    std::vector<Vector2> m_gls;
    std::vector<Hedron> m_hedrons;
    std::vector<Rat> m_rats;
    Vector2 m_startPos;
    Vector2 m_goalPos;
    Rat m_player;

    bool m_pushUp;
    bool m_pushDown;
    bool m_pushLeft;
    bool m_pushRight;

    bool m_pushShift;

    Rectangle m_up;
    Rectangle m_down;
    Rectangle m_left;
    Rectangle m_right;

    Rectangle m_shift;

    int m_state;
    float m_yScale;
    float m_upAngle;
    float m_targetAngle;
    int m_updir;
    int m_rotateCounter;
    int m_gotHedronIndex;

    int m_stateTmp;
    Rectangle m_resume;
    Rectangle m_quit;
    Rectangle m_show2DMaze;
    Rectangle m_rotate2DMaze;
    int m_poseSelect;
};

