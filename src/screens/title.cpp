#include "title.h"
#include "../app.h"
#include "./maze.h"

Title::Title()
{

}

void Title::init(){
    m_start.width = App::SCREEN_WIDTH / 3;
    m_start.height = App::SCREEN_HEIGHT /6;
    m_start.x = App::SCREEN_WIDTH*19/20 - m_start.width;
    m_start.y = App::SCREEN_HEIGHT*4/5;

    m_select = 0;
}

void Title::setSelect(int s){
    m_select = s;
    switch (m_select) {
    case 0:
    {
        m_tmp = m_app->config().playMode;
        break;
    }
    case 1:
    {
        m_tmp = m_app->config().mapSize;
        break;
    }
    case 2:
    {
        m_tmp = m_app->config().playerSpeed*500;
        break;
    }
    case 3:
    {
        m_tmp = m_app->config().ratsNum;
        break;
    }
    case 4:
    {
        m_tmp = m_app->config().ratsSpeed*500;
        break;
    }
    case 5:
    {
        m_tmp = (int)(100.0f / m_app->config().hedronRate);
        break;
    }
    case 6:
    {
        m_tmp = (int)(100.0f / m_app->config().glRate);
        break;
    }
    case 7:
    {
        m_tmp = m_app->config().show2DMaze == false ? 0 : 1;
        break;
    }
    case 8:
    {
        m_tmp = m_app->config().miniMapSize*100.0f / App::SCREEN_WIDTH;
        break;
    }
    case 9:
    {
        m_tmp = m_app->config().rotate2DMaze == false ? 0 : 1;
        break;
    }
    }
}

Screen* Title::update(){
    int ges = GetGestureDetected();

    int x = 0;
    int y = App::SCREEN_HEIGHT/40;
    int width = App::SCREEN_WIDTH * 0.5f;
    int height = App::SCREEN_HEIGHT/vSplit;

    if(IsKeyPressed(KEY_DOWN)){
        m_select = (m_select+12) % 11;
        setSelect(m_select);
    }

    if(IsKeyPressed(KEY_UP)){
        m_select = (m_select+10) % 11;
        setSelect(m_select);
    }

    if(ges != GESTURE_DRAG){
        int h, w, s=0;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
    }

    if(CheckCollisionPointRec(GetTouchPosition(0),m_start) && ges != GESTURE_DRAG){
        if(ges == GESTURE_TAP){
            return new Maze();
        }
        m_select = 10;

    }



    Vector2  drag = GetGestureDragVector();
    if(ges != GESTURE_DRAG){
        drag = {0.0f, 0.0f};
    }

    if(IsKeyPressed(KEY_LEFT)){
        m_tmp --;
        m_keyTime =0;
    }

    if(IsKeyDown(KEY_LEFT)){
        m_keyTime++;
        if(m_keyTime > 30 && (m_keyTime % 6) == 0)m_tmp--;
    }

    if(IsKeyPressed(KEY_RIGHT)){
        m_tmp ++;
        m_keyTime =0;
    }

    if(IsKeyDown(KEY_RIGHT)){
        m_keyTime++;
        if(m_keyTime > 30 && (m_keyTime % 6) == 0)m_tmp++;
    }


    switch (m_select) {
    case 0:
    {
        m_app->config().playMode =((int)(m_tmp + NUM_PLAY_MODES + drag.x*NUM_PLAY_MODES*3/20.0f)) % NUM_PLAY_MODES;

        break;
    }
    case 1:
    {
        m_app->config().mapSize =  drag.x*5/20.0f + m_tmp;
        m_app->config().mapSize = std::min(50, std::max(3, m_app->config().mapSize));
        m_app->config().ratsNum = std::min(m_app->config().mapSize*m_app->config().mapSize / 4 + 1, std::max(0, m_app->config().ratsNum));
        break;
    }
    case 2:
    {
        m_app->config().playerSpeed =  (drag.x*10/20.0f + m_tmp) / 500.0f;
        m_app->config().playerSpeed = std::min(0.2f, std::max(0.002f, m_app->config().playerSpeed));
        break;
    }
    case 3:
    {
        m_app->config().ratsNum =  drag.x*10/20.0f + m_tmp;
        m_app->config().ratsNum = std::min(m_app->config().mapSize*m_app->config().mapSize / 5 + 1, std::max(0, m_app->config().ratsNum));
        break;
    }
    case 4:
    {
        m_app->config().ratsSpeed =  (drag.x*100/20.0f + m_tmp) /500.0f;
        m_app->config().ratsSpeed  = std::min(0.2f, std::max(0.002f, m_app->config().ratsSpeed));
        break;
    }
    case 5:
    {
        m_app->config().hedronRate =  100.0f / (std::max(1.0f, drag.x*100/20.0f + m_tmp));
        m_app->config().hedronRate = std::min(100, std::max(4, m_app->config().hedronRate));
        break;
    }
    case 6:
    {
        m_app->config().glRate =  100.0f / (std::max(1.0f, drag.x*100/20.0f + m_tmp));
        m_app->config().glRate = std::min(100, std::max(4, m_app->config().glRate));
        break;
    }
    case 7:
    {
        m_app->config().show2DMaze =((int)(m_tmp + 2 + drag.x*2*3/20.0f)) % 2;
        break;
    }
    case 8:
    {
        m_app->config().miniMapSize =  (drag.x*50/20.0f + m_tmp) * App::SCREEN_WIDTH / 100.0f;
        m_app->config().miniMapSize = std::min<int>(App::SCREEN_WIDTH/3, std::max<int>(App::SCREEN_WIDTH/60, m_app->config().miniMapSize));
        break;
    }
    case 9:
    {
        m_app->config().rotate2DMaze =((int)(m_tmp + 2 + drag.x*2*3/20.0f)) % 2;
        break;
    }
    }


    return nullptr;
}

void Title::draw(){
    ClearBackground(BLACK);

    if(m_select == 10)DrawRectangleRec(m_start, GRAY);
    DrawRectangleLinesEx(m_start, 5, WHITE);
    DrawText("Start", m_start.x + m_start.width *0.1f, m_start.y + m_start.height* 0.2f, m_start.height *0.6, WHITE);

    int x = App::SCREEN_WIDTH/40;
    int y = App::SCREEN_HEIGHT/40;
    int height = App::SCREEN_HEIGHT/vSplit;


    const char* strs[] = {"Auto","Play", "Cheat"};
    int h=height, s=0;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Play Mode : %s", strs[m_app->config().playMode]),x, y, h, WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Map Size : %d", m_app->config().mapSize), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Player Speed: %d", (int)(m_app->config().playerSpeed*500)), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Num Rats : %d", m_app->config().ratsNum), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Rats Speed : %d", (int)(m_app->config().ratsSpeed*500)), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Hedron Freq : %.0f %%", (100.0f / m_app->config().hedronRate)), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("OpenGL Freq : %.0f %%", (100.0f / m_app->config().glRate)), x,y, h ,WHITE);
    y+=h;

    const char* strs2[]= {"Off", "On"};

    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Show 2D Maze : %s", strs2[m_app->config().show2DMaze ? 1:0]), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("2D Maze Size : %d", int(m_app->config().miniMapSize*100.0f / App::SCREEN_WIDTH)), x,y, h ,WHITE);
    y+=h;
    if(m_select == s) {
        DrawRectangle(0, y, App::SCREEN_WIDTH, h*2, GRAY);
    }
    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Flip 2D Maze : %s", strs2[m_app->config().rotate2DMaze ? 1:0]), x,y, h ,WHITE);
}
