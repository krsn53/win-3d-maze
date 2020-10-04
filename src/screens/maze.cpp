#include <string>
#include "maze.h"
#include <stdio.h>
#include <rlgl.h>
#include <raymath.h>

#include"../app.h"
#include "./objects/mazemesh.h"
#include "./title.h"
#include <GLFW/glfw3.h>

Maze::Maze()
{

}

std::string genShaderCode(const char* file, bool fragshader=true){
     std::string ret = std::string(LoadFileText(file));
     std::string utils = std::string(LoadFileText("res/utils.glsl"));
     int s = 0;
     int i;



     while((i = ret.find("#include", s)) < ret.size()){
        s = ret.find("\n", i);
        std::string inc = ret.substr(i,s-i);

        int db = ret.find("\"",i+8);
        std::string inc_file = "res/" + ret.substr(db+1, ret.find("\"", db+1) - (db+1));
        std::string inc_data = LoadFileText(inc_file.data());

        ret.replace(i, inc.size(), inc_data);
     }

     ret =
(fragshader ?
R"(
#version 100
precision mediump float;
 )"
:
R"(
#version 100
)"
             )
+ utils + ret;

     return ret;
}




void Maze::start(){
    m_state = START;
    m_yScale = 0.0f;
    m_upAngle = 0.0f;

    m_updir = 1;

    m_upAngle = 0.0f;

    const int X = m_app->config().mapSize, Z = m_app->config().mapSize;

    m_map.set(X,Z);
    m_map.generate();

    int x =0;
    int z=0;

    m_startPos.x = x= random() % X;
    m_startPos.y = z =0;
    m_startPos.x += 0.5f;
    m_startPos.y += 0.5f;
    m_map[z][x].item = true;


    m_goalPos.x = x= random() % X;
    m_goalPos.y = z = Z-1;
    m_goalPos.x += 0.5f;
    m_goalPos.y += 0.5f;
    m_map[z][x].item = true;

    m_player.angle = 0;
    m_player.pos = {m_startPos.x, -0.01f};
    m_player.rotateFrame = -1;
    m_player.dir = DIRECTION_FRONT;
    m_player.aVel = 0;
    m_player.aTarget = 0.0f;

    m_pushUp = m_pushDown = m_pushLeft = m_pushRight = m_pushShift = false;


    m_camera.position = Vector3{WIDTH*m_player.pos.x, 0.0f, WIDTH*m_player.pos.y};
    m_camera.target = Vector3{m_camera.position.x + sinf(DEG2RAD*(m_player.angle)), 0.0f, m_camera.position.z + cosf(DEG2RAD*(m_player.angle))};
    m_camera.up = Vector3{0, 1.0f, 0};
    m_camera.fovy = 60.0f;
    m_camera.type = CAMERA_PERSPECTIVE;
    m_targetAngle = 0.0f;

    m_floor = LoadModelFromMesh( genMeshFloor(m_atlas, X, Z,
                                              Rectangle{128,0, 128,128},
                                              Rectangle{256, 0, 128,128}));
    m_floor.materials[0].maps[MAP_DIFFUSE].texture = m_atlas;

    m_wall = LoadModelFromMesh( genMeshWall(m_atlas, m_map, Rectangle{0, 0, 128,128}, Rectangle{0.0f, 128.0f, 256.0f, 256.0f}));
    m_wall.materials[0].maps[MAP_DIFFUSE].texture = m_atlas;

    int n = random() % ((X*Z+m_app->config().glRate - 2)/ m_app->config().glRate) + 1;
    m_gls.reserve(n);
    for(int i=0; i< n; i++) {

        do {
            x =  (random()) % X ;
            z = (random()) % Z;
        }
        while(m_map[z][x].item);
        float xp = x + 0.5f;
        float zp = z + 0.5f;
        m_map[z][x].item = true;
        m_gls.emplace_back(Vector2{xp, zp});
    }

    n = (X*Z - 2)/ m_app->config().hedronRate + 1;
    m_hedrons.reserve(n);
    for(int i=0; i< n; i++) {
        do {
            x =  random() % X ;
            z = random() % Z;
        }
        while(m_map[z][x].item);
        int type = random() % Hedron::NUM_TYPES;
        m_map[z][x].item = true;
        float xp = x+ 0.5f;
        float zp = z + 0.5f;

        m_hedrons.emplace_back(Hedron{Vector2{xp, zp}, type, float(random() % 360)});
    }

    n = m_app->config().ratsNum;
    m_rats.reserve(n);
    for(int i=0; i< n; i++) {
        do{
            x = random() % X;
            z = random() % Z;
        }while(m_map[z][x].item);
        m_map[z][x].item = true;
        float xp = x + 0.5f;
        float zp = z + 0.5f;
        float a;
        int dir;
        if(!m_map.frontIsWall(x, z)) {
            dir = DIRECTION_FRONT;
            a = 0.0f;
        }
        else if(!m_map.backIsWall(x,z)) {
            dir = DIRECTION_BACK;
            a = 180.0f;
        }
        else if(!m_map.rightIsWall(x, z)) {
            dir =DIRECTION_RIGHT;
            a = 270.0f;
        }
        else {
            dir = DIRECTION_LEFT;
            a = 90.0f;
        }

        m_rats.emplace_back(Rat{Vector2{xp, zp}, dir, 0, a, 0.0f, a});
    }
}

void Maze::rotate(){
    m_state = ROTATE;
    m_rotateCounter  = 1.0f/ m_app->config().playerSpeed;
}

void Maze::goal(){
    m_state = END;
}


void Maze::init(){
    m_atlas =LoadTexture("res/atlas.png");

    m_tetra = LoadModelFromMesh(genMeshTetrahedron());
    m_octa = LoadModelFromMesh(genMeshOctahedron());
    m_dodeca = LoadModelFromMesh(genMeshDodecahedron());
    m_icosa = LoadModelFromMesh(genMeshIcosahedron());
    m_hedronShader = LoadShaderCode(genShaderCode("res/normal.vert", false).data(), genShaderCode("res/normal.frag").data());
    m_alphaclip = LoadShaderCode(genShaderCode("res/base.vert", false).data(), genShaderCode("res/alphaclip.frag").data());

    m_hedronShader.locs[LOC_MATRIX_MODEL] = GetShaderLocation(m_hedronShader, "matModel");
    m_tetra.materials[0].shader = m_hedronShader;
    m_octa.materials[0].shader = m_hedronShader;
    m_dodeca.materials[0].shader = m_hedronShader;
    m_icosa.materials[0].shader = m_hedronShader;

    m_up.width = App::SCREEN_WIDTH *0.15f;
    m_up.height = m_up.width;
    m_up.x = 10 + m_up.width*0.66f;
    m_up.y = App::SCREEN_HEIGHT - (m_up.height)*1.66f - 10;

    m_down.width = m_up.width;
    m_down.height = m_up.height*0.66;
    m_down.x = 10+ m_up.width*0.66f;
    m_down.y = App::SCREEN_HEIGHT - m_up.height*0.66f- 10;

    m_left.width = m_up.width*0.66f;
    m_left.height = m_up.height*1.66f;
    m_left.x = + 10;
    m_left.y = App::SCREEN_HEIGHT - m_up.height*1.66f - 10;

    m_right.width = m_up.width*0.55f;
    m_right.height = m_up.height*1.66f;
    m_right.x = 10+ m_up.width*1.66f;
    m_right.y = App::SCREEN_HEIGHT - m_up.height*1.66f - 10;

    m_shift.width = m_up.width *2;
    m_shift.height = m_up.height;
    m_shift.x = App::SCREEN_WIDTH - 10 - m_shift.width;
    m_shift.y = App::SCREEN_HEIGHT - 10 - m_shift.height;

    m_resume.width = App::SCREEN_WIDTH / 3;
    m_resume.height = App::SCREEN_HEIGHT / 5;
    m_resume.x = App::SCREEN_WIDTH / 10;
    m_resume.y = App::SCREEN_HEIGHT / 4 - m_resume.height*0.5f;

    m_quit.width = App::SCREEN_WIDTH / 3;
    m_quit.height = App::SCREEN_HEIGHT / 5;
    m_quit.x = App::SCREEN_WIDTH*2 / 10 + App::SCREEN_WIDTH/3;
    m_quit.y = App::SCREEN_HEIGHT / 4 - m_quit.height*0.5f;

    m_show2DMaze.height = App::SCREEN_HEIGHT / 5;
    m_show2DMaze.width = m_show2DMaze.height;
    m_show2DMaze.x = App::SCREEN_WIDTH / 10;
    m_show2DMaze.y = App::SCREEN_HEIGHT*2 / 4 - m_show2DMaze.height*0.5f;

    m_rotate2DMaze.height = App::SCREEN_HEIGHT / 5;
    m_rotate2DMaze.width = m_rotate2DMaze.height;
    m_rotate2DMaze.x = App::SCREEN_WIDTH / 10;
    m_rotate2DMaze.y = App::SCREEN_HEIGHT*3 / 4 - m_show2DMaze.height*0.5f;

    start();
}

void ratUpdate(Rat& i, MazeMap& map, float speed){
    if(i.rotateFrame > 0){
        i.rotateFrame--;
        i.angle += i.aVel*speed / (PI*0.25f);
    } else {
        i.aVel = 0.0f;
    }

    int x1 = i.pos.x;
    int z1 = i.pos.y;

    if(fabs(i.aVel) < 180){
        Vector2 vel;
        vel.x =sinf(DEG2RAD*i.angle)*speed;
        vel.y = cosf(DEG2RAD*i.angle)*speed;

        i.pos.x +=  vel.x;
        i.pos.y +=  vel.y;
    }

    int x = i.pos.x ;
    int z = i.pos.y ;

    if(x1 != x || z1 != z || i.rotateFrame==-1){
        i.angle = i.aTarget;

        i.aVel = 0.0f;
        float da=0.0f;


            da = 0.0f;

            bool col[NUM_DIRECTIONS] =
            {
                map.frontIsWall(x, z),
                map.rightIsWall(x, z),
                map.backIsWall( x, z),
                map.leftIsWall( x,z),
            };
            if(!col[(i.dir+(NUM_DIRECTIONS+1))% NUM_DIRECTIONS]){
                i.dir = (i.dir+(NUM_DIRECTIONS+1))% NUM_DIRECTIONS;
                da -= 90.0f;
            }
            else if(col[i.dir]){
                int c =0;
                do{
                    i.dir = (i.dir+(NUM_DIRECTIONS-1))% NUM_DIRECTIONS;
                    da += 90.0f;
                }while(col[i.dir] && ++c < NUM_DIRECTIONS) ;
            }


        i.aVel+=da;

        if(i.aVel != 0.0f){
            i.aTarget = i.angle + i.aVel;
            i.rotateFrame = 1.0f/ (speed) * PI* 0.25f + 1.0f;
        }
    }
}

void Maze::pose(){
    m_stateTmp = m_state;
    m_state = POSE;
    m_poseSelect = -1;

}


Screen* Maze::update(){
    const int X = m_map.x(), Z = m_map.z();

    int ges = GetGestureDetected();
    Vector2 gesPoint =GetTouchPosition(0);


    if(m_state != POSE &&
      (ges == GESTURE_SWIPE_UP ||
       IsKeyPressed(KEY_SPACE))){
        pose();
    }

    if(IsKeyPressed(KEY_ONE)){
        m_app->config().show2DMaze = !m_app->config().show2DMaze;
    }

    if(m_state == START){
        m_yScale = m_yScale + 1.0f /FPS;
        if(m_yScale >= 1.0f) {
            m_yScale = 1.0f;
            m_state = MAIN;
        }
    }
    else if(m_state == POSE){

        if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
            if(m_poseSelect== 0) {
                m_state = m_stateTmp;
            }
            else if(m_poseSelect== 1) {
                return new Title();
            }
            else if(m_poseSelect== 2){
                m_app->config().show2DMaze = !m_app->config().show2DMaze;
            }
            else if(m_poseSelect== 3){
                m_app->config().rotate2DMaze = !m_app->config().rotate2DMaze;
            }
        }

        if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
            m_poseSelect = (m_poseSelect+1)%4;
        }

        if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
            m_poseSelect = (m_poseSelect+3)%4;
        }

        if(CheckCollisionPointRec(gesPoint , m_resume)){
            if(ges == GESTURE_TAP){
                m_state = m_stateTmp;
            }
            m_poseSelect = 0;
        }
        if(CheckCollisionPointRec(gesPoint, m_quit)){
            if(ges == GESTURE_TAP){
                return new Title();
            }
            m_poseSelect =1;
        }
        if(CheckCollisionPointRec(gesPoint, m_show2DMaze)){
            if(ges == GESTURE_TAP){
                m_app->config().show2DMaze = !m_app->config().show2DMaze;
            }
            m_poseSelect =2;
        }
        if(CheckCollisionPointRec(gesPoint, m_rotate2DMaze)){
            if(ges == GESTURE_TAP){
                  m_app->config().rotate2DMaze = !m_app->config().rotate2DMaze;
            }
            m_poseSelect =3;
        }
    }
    else if(m_state == MAIN) {
        float dx = sinf(DEG2RAD*(m_player.angle));
        float dz = cosf(DEG2RAD*(m_player.angle));

        float cosTarget = cosf(m_targetAngle*DEG2RAD);
        float sinTarget = sinf(m_targetAngle*DEG2RAD);


        if(m_app->config().playMode == AUTO_MODE){
            ratUpdate(m_player, m_map, m_app->config().playerSpeed);

            m_camera.position.x = m_player.pos.x*WIDTH;
            m_camera.position.z = m_player.pos.y*WIDTH;
        }
        else if(m_app->config().playMode == PLAY_MODE || m_app->config().playMode == CHEAT_MODE){
            float speed = m_app->config().playerSpeed * WIDTH;


            Vector2 velocity = Vector2{0.0f, 0.0f};

            m_pushUp = m_pushDown = m_pushLeft = m_pushRight =  m_pushShift = false;

            if(IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
                m_pushUp = true;
            }

            if(IsKeyDown(KEY_DOWN)|| IsKeyDown(KEY_S)) {
                m_pushDown = true;
            }

            if(IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A)){
                m_pushLeft = true;
            }

            if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)){
                m_pushRight = true;
            }
            if(IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)){
                m_pushShift = true;
            }

            int pn = GetTouchPointsCount();
            for(int i=0; i< pn;i++){
                Vector2 point = GetTouchPosition(i);
                if(CheckCollisionPointRec(point, m_up)){
                    m_pushUp = true;
                }
                if(CheckCollisionPointRec(point, m_down)){
                    m_pushDown = true;
                }
                if(CheckCollisionPointRec(point, m_left)){
                     m_pushLeft = true;
                }
                if(CheckCollisionPointRec(point, m_right)){
                    m_pushRight = true;
                }
                if(CheckCollisionPointRec(point, m_shift)){
                    m_pushShift = true;
                }
            }

            if( m_app->config().playMode == PLAY_MODE){

                if( !m_pushShift){

                    if(m_pushUp){
                        velocity.x = dx*speed;
                        velocity.y = dz*speed;
                    }
                    if(m_pushDown){
                        velocity.x = -dx*speed*0.75f;
                        velocity.y = -dz*speed*0.75f;
                    }
                } else {
                    if(m_pushUp){
                        m_targetAngle +=90 * speed / (PI*0.25f)*m_updir;
                        m_targetAngle = std::min(80.0f, m_targetAngle);
                    }
                    if(m_pushDown){
                      m_targetAngle -=90 * speed / (PI*0.25f)*m_updir;
                      m_targetAngle = std::max(-80.0f, m_targetAngle);

                    }

                }
                if(m_pushLeft){
                    m_player.angle+=90 * speed / (PI*0.25f)*m_updir;
                }
                if(m_pushRight){
                    m_player.angle-=90 * speed / (PI*0.25f)*m_updir;
                }
            }
            else {
                if( !m_pushShift){

                    if(m_pushUp){
                        velocity.x = dx*speed*cosTarget*2;
                        velocity.y = dz*speed*cosTarget*2;
                        m_camera.position.y += speed*sinTarget*2;
                    }
                    if(m_pushDown){
                        velocity.x = -dx*speed*cosTarget*2;
                        velocity.y = -dz*speed*cosTarget*2;
                        m_camera.position.y -= speed*sinTarget*2;
                    }
                } else {
                    if(m_pushUp){
                        m_targetAngle +=90 * speed / (PI*0.25f)*m_updir;
                        m_targetAngle = std::min(89.0f, m_targetAngle);
                    }
                    if(m_pushDown){
                      m_targetAngle -=90 * speed / (PI*0.25f)*m_updir;
                      m_targetAngle = std::max(-89.0f, m_targetAngle);

                    }

                }
                if(m_pushLeft){
                    m_player.angle+=90 * speed / (PI*0.25f)*m_updir;
                }
                if(m_pushRight){
                    m_player.angle-=90 * speed / (PI*0.25f)*m_updir;
                }
            }


            if(m_app->config().playMode == PLAY_MODE){

                float signx = velocity.x > 0.0f ? 1.0f : -1.0f;
                float signy = velocity.y > 0.0f ? 1.0f : -1.0f;

                int x1 = (m_camera.position.x) / WIDTH;
                int z1 = (m_camera.position.z) / WIDTH ;

                float x2f = m_camera.position.x+ signx*WIDTH*0.25f + velocity.x;
                float z2f = m_camera.position.z +signy*WIDTH*0.25f + velocity.y;

                int x2 = x2f / WIDTH;
                int z2 =  z2f / WIDTH;

                bool left = m_map.leftIsWall(x1, z1) ;
                bool right = m_map.rightIsWall(x1, z1);
                bool front = m_map.frontIsWall(x1, z1);
                bool back = m_map.backIsWall( x1, z1);

                if(x2f < 0 || x2f > X*WIDTH) velocity.x = 0;
                else if( x1 != x2){
                    if(x2 > x1) {
                        if(left){
                            velocity.x = -m_camera.position.x - signx*WIDTH*0.25f + WIDTH*(x1+1);
                        }
                    } else {
                        if(right){
                            velocity.x = - m_camera.position.x - signx*WIDTH*0.25f + WIDTH*(x1);
                        }
                    }
                }
                if(z2f < 0.0f|| z2f > Z*WIDTH) velocity.y = 0;
                else if(z1 != z2) {
                    if(z2 > z1) {
                        if(front){
                            velocity.y = -m_camera.position.z - signy*WIDTH*0.25f + WIDTH*(z1+1);
                        }
                    } else {
                       if(back){
                            velocity.y =  -m_camera.position.z - signy*WIDTH*0.25f + WIDTH*(z1);
                        }
                    }
                }

                if(x1 != x2 && z1 != z2) {
                    bool left2= m_map.leftIsWall(x2, z2) ;
                    bool right2 = m_map.rightIsWall(x2, z2);
                    bool front2 = m_map.frontIsWall(x2, z2);
                    bool back2 = m_map.backIsWall( x2, z2);

                    if(x2 > x1) {
                        if(z2 > z1){
                            if((right2 || back2 )&& !left && !front){
                                if(back2)velocity.x = -m_camera.position.x - signx*WIDTH*0.25f + WIDTH*(x1+1);
                                if(right2)velocity.y = -m_camera.position.z - signy*WIDTH*0.25f + WIDTH*(z1+1);
                            }
                        } else {
                            if((right2 || front2) && !left && !back){
                                if(front2)velocity.x = -m_camera.position.x - signx*WIDTH*0.25f + WIDTH*(x1+1);
                                if(right2)velocity.y = -m_camera.position.z - signy*WIDTH*0.25f + WIDTH*(z1);
                            }
                        }
                    } else {
                        if(z2 > z1){
                            if((left2 || back2) && !right && !front){
                                if(back2)velocity.x = -m_camera.position.x - signx*WIDTH*0.25f + WIDTH*(x1);
                                if(left2)velocity.y = -m_camera.position.z - signy*WIDTH*0.25f + WIDTH*(z1+1);
                            }
                        } else {
                            if((left2 || front2) && !right && !back){
                                if(front2)velocity.x = -m_camera.position.x - signx*WIDTH*0.25f + WIDTH*(x1);
                                if(back2)velocity.y = -m_camera.position.z - signy*WIDTH*0.25f + WIDTH*(z1);
                            }
                        }
                    }
                }
            }
            m_camera.position.x += velocity.x;
            m_camera.position.z += velocity.y;
        }


        m_camera.target = Vector3{m_camera.position.x + dx*cosTarget, m_camera.position.y+sinTarget, m_camera.position.z + dz*cosTarget};


        auto e = m_hedrons.size();
        for(auto i = 0; i< e; i++){
            float dx = m_hedrons[i].pos.x - m_camera.position.x/WIDTH;
            float dy = m_hedrons[i].pos.y - m_camera.position.z/WIDTH;
            if(dx*dx + dy*dy < 0.25f && fabs(m_camera.position.y) < 0.25f){
               m_gotHedronIndex = i;
                rotate();
            }
        }


        {
            float dx = m_goalPos.x - m_camera.position.x/WIDTH;
            float dy = m_goalPos.y - m_camera.position.z/WIDTH;
            if(dx*dx + dy*dy < 0.25f && fabs(m_camera.position.y) < 0.25f){
                goal();
            }
        }

        for(auto&i:m_hedrons){
            i.angle+= 120 / FPS;
        }

        for(auto &i: m_rats){
            ratUpdate(i, m_map, m_app->config().ratsSpeed);
        }
    }
    else if(m_state == ROTATE){
        m_upAngle += 180*m_app->config().playerSpeed;
        m_rotateCounter--;
        if(m_rotateCounter ==0){
            m_upAngle = int(m_upAngle) - int (m_upAngle) / 360 * 360;
            m_upAngle = int(m_upAngle+90)/180 * 180;
            m_updir = m_updir == 1 ? -1: 1;
            m_state = MAIN;
            m_hedrons[m_gotHedronIndex] = m_hedrons.back();
            m_hedrons.pop_back();
        }

        for(auto&i:m_hedrons){
            i.angle+= 120 / FPS;
        }
    }
    else if(m_state == END){
        m_yScale = m_yScale - 1.0f /FPS;
        if(m_yScale <= 0.0f) {
            m_yScale = 0.0f;

            m_gls.clear();
            m_hedrons.clear();
            m_rats.clear();

            UnloadModel(m_wall);
            UnloadModel(m_floor);

            start();
        }
    }

    return nullptr;
}

void Maze::draw(){
    glEnable(GL_CULL_FACE);

    ClearBackground(BLACK);
    BeginMode3D(m_camera);
        rlMatrixMode( RL_PROJECTION);
        rlRotatef(m_upAngle, 0.0f, 0.0f, 1.0f);
         rlMatrixMode(RL_MODELVIEW);

        rlPushMatrix();
        rlScalef(WIDTH, 1, WIDTH);

        DrawModel(m_floor, Vector3{0,0,0}, 1.0, WHITE);
        rlPushMatrix();
        rlTranslatef(0.0f, -(1.0f - m_yScale)/2.0f, 0.0f);
        rlScalef(1.0f, m_yScale, 1.0f);

        DrawModel(m_wall, Vector3{0,0,0}, 1.0, WHITE);


        for(auto &i: m_hedrons){
            switch (i.type) {
            case Hedron::TETRA:
                DrawModelEx(m_tetra, Vector3{i.pos.x, -0.25f, i.pos.y}, Vector3{0.5f, 1.0f, 0.5f}, i.angle, Vector3{0.4f, 0.4f, 0.4f}, WHITE );
                break;
            case Hedron::OCTA:
                DrawModelEx(m_octa, Vector3{i.pos.x, -0.25f, i.pos.y}, Vector3{0.5f, 1.0f, 0.5f}, i.angle, Vector3{0.4f, 0.4f, 0.4f}, WHITE );
                break;
            case Hedron::DODECA:
                DrawModelEx(m_dodeca, Vector3{i.pos.x, -0.25f, i.pos.y}, Vector3{0.5f, 1.0f, 0.5f},i.angle, Vector3{0.4f, 0.4f, 0.4f}, WHITE );
                break;
            case Hedron::ICOSA:
                DrawModelEx(m_icosa, Vector3{i.pos.x, -0.25f, i.pos.y}, Vector3{0.5f, 1.0f, 0.5f}, i.angle, Vector3{0.4f, 0.4f, 0.4f}, WHITE );
            }
        }
        BeginBlendMode(BLEND_ALPHA);
        BeginShaderMode(m_alphaclip);

        for(auto& i: m_rats){
            DrawBillboardRec(m_camera, m_atlas, Rectangle{0.0f, 384.0f, 256.0f, 128.0f}, Vector3{i.pos.x , -0.35f, i.pos.y}, 0.5f, WHITE);
        }

        rlPushMatrix();

        rlScalef(1.0f, WIDTH, 1.0f);
        glDepthMask(false);
            for(auto& i: m_gls){
                DrawBillboardRec(m_camera, m_atlas, Rectangle{256.0f, 256.0f, 256.0f, 64.0f}, Vector3{i.x , 0.0f, i.y}, 1.0f, Color{255,255,255,127});
            }

        rlPopMatrix();

        //goal
        DrawBillboardRec(m_camera, m_atlas, Rectangle{384.0f, 0.0f, 128.0f, 128.0f}, Vector3{m_goalPos.x, 0.0f, m_goalPos.y}, 0.4f, Color{255,255,255,127});
        rlPushMatrix();
        rlScalef(1.0f, WIDTH, 1.0f);
        //start
        DrawBillboardRec(m_camera, m_atlas, Rectangle{256.0f, 128.0f, 256.0f, 128.0f}, Vector3{m_startPos.x , 0.0f, m_startPos.y}, 0.4f, Color{255,255,255,127});
        rlPopMatrix();
        glDepthMask(true);
        rlPopMatrix();

        EndShaderMode();
        EndBlendMode();
    rlPopMatrix();
    EndMode3D();

    if(m_app->config().show2DMaze){
        //2D
        rlPushMatrix();

        rlTranslatef(App::SCREEN_WIDTH/2.0f, App::SCREEN_HEIGHT/2.0f, 0.0f);

        glDisable(GL_CULL_FACE);
        if(m_app->config().rotate2DMaze){
            float scale = cosf(m_upAngle*DEG2RAD);
            rlScalef(scale,1.0f, 1.0f);
        }

        const int X = m_map.x(), Z = m_map.z();

        int size = m_app->config().miniMapSize;
        int view = sqrtf(App::SCREEN_WIDTH*App::SCREEN_WIDTH + App::SCREEN_HEIGHT*App::SCREEN_HEIGHT) / size + 2;
        float tick = size / 64.0f + 1.0f;
        int x = -m_camera.position.x / WIDTH* size;
        int z = -m_camera.position.z / WIDTH * size;

        int px = -x/size;
        int pz = -z/size;

        int beginX = std::max(0, px-view/2);
        int beginZ = std::max(0, pz-view/2);

        int endX = std::min(X, px+view/2);
        int endZ = std::min(Z, pz+view/2);

        rlPushMatrix();


            rlRotatef(m_player.angle + 180, 0,0,1);
            DrawRectangleLinesEx({(float)x, (float)z, (float)X*size, (float)Z*size}, tick, WHITE);

            float sx, sz;

            for(int i=beginZ; i<endZ; i++){
                for(int j=beginX; j<endX; j++){
                    sx = j*size;
                    sz = i*size;
                    if(m_map[i][j].right) {
                        DrawLineEx({x + sx,  z + sz},
                                 {x + sx, z + sz +size},(int)tick, WHITE);
                    }
                    if(m_map[i][j].back) {
                        DrawLineEx({x + sx,  z + sz},
                                {x + sx + size, z + sz} ,(int)tick, WHITE);
                    }
                }
            }
            //gls
            for(auto &i: m_gls){
                sx = i.x*size;
                sz = i.y*size;

                DrawTriangle(Vector2{sx + x, sz + z+size/3.0f},
                             Vector2{sx + x+size/6.0f, sz + z-size/3.0f},
                             Vector2{sx + x-size/6.0f, sz + z-size/3.0f}, Color{255,255,255,255});
            }


            //hedrons
            for(auto &i: m_hedrons){
                sx = i.pos.x*size;
                sz = i.pos.y*size;
                rlPushMatrix();
                rlTranslatef(sx+x, sz+z, 0.0f);
                rlRotatef(-i.angle, 0,0,1);
                DrawTriangle(Vector2{0, size/6.0f},
                             Vector2{+size/12.0f,-size/6.0f},
                             Vector2{-size/12.0f,-size/6.0f}, Color{255,255,255,255});
                rlPopMatrix();
            }

            //rats
            for(auto &i: m_rats){
                sx = i.pos.x*size;
                sz = i.pos.y*size;
                rlPushMatrix();
                rlTranslatef(sx+x, sz+z, 0.0f);
                rlRotatef(-i.angle, 0,0,1);
                DrawTriangle(Vector2{0, size/6.0f},
                             Vector2{+size/12.0f,-size/6.0f},
                             Vector2{-size/12.0f,-size/6.0f}, Color{236,136,16,255});
                rlPopMatrix();
            }

            sx = m_startPos.x*size;
            sz = m_startPos.y*size;

            //start
            DrawTriangle(Vector2{sx + x, sz + z+size/6.0f},
                         Vector2{sx + x+size/12.0f, sz + z-size/6.0f},
                         Vector2{sx + x-size/12.0f, sz + z-size/6.0f}, Color{255,0,0,255});

            //goal
            sx = m_goalPos.x*size;
            sz = m_goalPos.y*size;
            //start
            DrawTriangle(Vector2{sx + x, sz + z+size/6.0f},
                         Vector2{sx + x+size/12.0f, sz + z-size/6.0f},
                         Vector2{sx + x-size/12.0f, sz + z-size/6.0f}, Color{0,255,0,255});

            rlPopMatrix();

            //player
            DrawTriangle(Vector2{0, -size/4.0f}, Vector2{-size/8.0f, size/4.0f}, Vector2{size/8.0f, size/4.0f}, Color{0,0,255,255});
             rlPopMatrix();
        }

    if(m_app->config().playMode == PLAY_MODE || m_app->config().playMode == CHEAT_MODE){
        if(m_pushUp){
            DrawRectangleRec(m_up,  Color{0,0,0, 64});
        }
        if(m_pushDown){
            DrawRectangleRec(m_down,  Color{0,0,0, 64});
        }
        if(m_pushLeft){
            DrawRectangleRec(m_left,  Color{0,0,0, 64});
        }
        if(m_pushRight){
            DrawRectangleRec(m_right,  Color{0,0,0, 64});
        }
        DrawRectangleLinesEx(m_up, 2.0f, Color{255,255,255, 127});
        DrawTriangle(
            {m_up.x+m_up.width/2, m_up.y+m_up.height/4},
            {m_up.x+m_up.width/4, m_up.y+3*m_up.height/4},
            {m_up.x+3*m_up.width/4, m_up.y+3*m_up.height/4}, Color{255,255,255, 127});

        DrawRectangleLinesEx(m_down,  2.0f, Color{255,255,255, 127});
        DrawTriangle(
            {m_down.x+m_down.width/2, m_down.y+3*m_down.height/4},
            {m_down.x+3*m_down.width/4, m_down.y+m_down.height/4},
            {m_down.x+m_down.width/4, m_down.y+m_down.height/4}, Color{255,255,255, 127});

        DrawRectangleLinesEx(m_left,  2.0f, Color{255,255,255, 127});
        DrawTriangle(
            {m_left.x+m_left.width/4, m_left.y+m_left.height/2},
            {m_left.x+3*m_left.width/4, m_left.y+3*m_left.height/4},
            {m_left.x+3*m_left.width/4, m_left.y+m_left.height/4}, Color{255,255,255, 127});

        DrawRectangleLinesEx(m_right, 2.0f, Color{255,255,255, 127});
        DrawTriangle(
            {m_right.x+3*m_right.width/4, m_right.y+m_right.height/2},
            {m_right.x+m_right.width/4, m_right.y+m_right.height/4},
            {m_right.x+m_right.width/4, m_right.y+3*m_right.height/4}, Color{255,255,255, 127});

        if(m_pushShift){
            DrawRectangleRec(m_shift, Color{0,0,0, 64});
        }
        DrawRectangleLinesEx(m_shift, 2.0f, Color{255,255,255, 127});
        DrawText("Shift", m_shift.x + m_shift.width / 10.0f, m_shift.y + m_shift.height/10.0f, m_shift.height *0.5f, Color{255,255,255, 127});


    }

    if(m_state == POSE){

        DrawRectangle(50.0f, 20.0f, App::SCREEN_WIDTH-100.0f, App::SCREEN_HEIGHT-40.0f, Color{0,0,0, 200});

        DrawRectangleRec(m_resume,  m_poseSelect == 0 ?Color{255,255,255, 200} : Color{127,127,127, 200});
        DrawRectangleLinesEx(m_resume, 5, Color{0,0,0, 255});
        DrawText("Resume", m_resume.x + m_resume.width*0.1f, m_resume.y+ m_resume.height*0.2f, m_resume.height*0.6f, Color{0,0,0, 200});

        DrawRectangleRec(m_quit, m_poseSelect == 1 ?Color{255,255,255, 200} : Color{127,127,127, 200});
        DrawRectangleLinesEx(m_quit, 5, Color{0,0,0, 255});
        DrawText("Quit", m_quit.x + m_quit.width*0.1f, m_quit.y+ m_quit.height*0.2f, m_quit.height*0.6f, Color{0,0,0, 200});

        DrawRectangleRec(m_show2DMaze, m_poseSelect == 2 ?Color{255,255,255, 200} : Color{127,127,127, 200});
        DrawRectangleLinesEx(m_show2DMaze, 5, Color{0,0,0, 255});
        DrawText("Show 2D Maze", m_show2DMaze.x + m_show2DMaze.width*1.1f , m_show2DMaze.y+ m_show2DMaze.height*0.2f, m_show2DMaze.height*0.6f, Color{255,255,255, 200});
        if(m_app->config().show2DMaze) {
            DrawRectangle(m_show2DMaze.x + m_show2DMaze.width *0.1f,
                                 m_show2DMaze.y + m_show2DMaze.height *0.1f,
                                 m_show2DMaze.width *0.8f,
                                  m_show2DMaze.height *0.8f, Color{0,0,0, 255});


            DrawRectangleRec(m_rotate2DMaze, m_poseSelect == 3 ?Color{255,255,255, 200} : Color{127,127,127, 200});
            DrawRectangleLinesEx(m_rotate2DMaze, 5, Color{0,0,0, 255});
            DrawText("Flip 2D Maze", m_rotate2DMaze.x + m_rotate2DMaze.width*1.1f , m_rotate2DMaze.y+ m_rotate2DMaze.height*0.2f, m_rotate2DMaze.height*0.6f, Color{255,255,255, 200});
             DrawText("(When Y axis rotate)", m_rotate2DMaze.x + m_rotate2DMaze.width*1.1f , m_rotate2DMaze.y+ m_rotate2DMaze.height*0.8f, m_rotate2DMaze.height*0.2f, Color{255,255,255, 200});
            if(m_app->config().rotate2DMaze) {
                DrawRectangle(m_rotate2DMaze.x + m_rotate2DMaze.width *0.1f,
                                     m_rotate2DMaze.y + m_rotate2DMaze.height *0.1f,
                                     m_rotate2DMaze.width *0.8f,
                                      m_rotate2DMaze.height *0.8f, Color{0,0,0, 255});
            }
        }


    }


}


void Maze::remove(){
    UnloadTexture(m_atlas);
    UnloadModel(m_floor);
    UnloadShader(m_hedronShader);

    UnloadModel(m_tetra);
    UnloadModel(m_octa);
    UnloadModel(m_dodeca);
    UnloadModel(m_icosa);
}
