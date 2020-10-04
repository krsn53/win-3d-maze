#include "title.h"
#include "../app.h"
#include "./maze.h"

Title::Title()
{

}

void Title::init(){
    m_tmp = 0;
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

    int h, w, s=0;
    for(int i=0; i<11; i++){
        w=m_select == s ? width*2 : width;
        h=m_select == s++ ? height*2 : height;
        if(CheckCollisionPointRec(GetTouchPosition(0), Rectangle{(float)x,(float)y, (float)w, (float)h})){
            setSelect(s-1);
        }
        y+=h;

    }

    int point = GetTouchX();
    if(point < h*4 && (ges & GESTURE_TAP) == GESTURE_TAP ) {
        m_tmp --;
    }

    if(point > App::SCREEN_WIDTH - h*4 && (ges & GESTURE_TAP) == GESTURE_TAP) {
        m_tmp ++;
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


    if(m_select == 10 && ( (ges & GESTURE_TAP) == GESTURE_TAP ||IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))){
        return new Maze();
    }

    switch (m_select) {
    case 0:
    {
        m_app->config().playMode =((int)(m_tmp + NUM_PLAY_MODES)) % NUM_PLAY_MODES;

        break;
    }
    case 1:
    {
        m_app->config().mapSize =  m_tmp;
        m_app->config().mapSize = std::min(50, std::max(3, m_app->config().mapSize));
        m_app->config().ratsNum = std::min(m_app->config().mapSize*m_app->config().mapSize / 4 + 1, std::max(0, m_app->config().ratsNum));
        break;
    }
    case 2:
    {
        m_app->config().playerSpeed =  (m_tmp) / 500.0f;
        m_app->config().playerSpeed = std::min(0.2f, std::max(0.002f, m_app->config().playerSpeed));
        break;
    }
    case 3:
    {
        m_app->config().ratsNum =  m_tmp;
        m_app->config().ratsNum = std::min(m_app->config().mapSize*m_app->config().mapSize / 5 + 1, std::max(0, m_app->config().ratsNum));
        break;
    }
    case 4:
    {
        m_app->config().ratsSpeed =  (m_tmp) /500.0f;
        m_app->config().ratsSpeed  = std::min(0.2f, std::max(0.002f, m_app->config().ratsSpeed));
        break;
    }
    case 5:
    {
        m_app->config().hedronRate =  100.0f / (std::max(1.0f,  m_tmp));
        m_app->config().hedronRate = std::min(100, std::max(4, m_app->config().hedronRate));
        break;
    }
    case 6:
    {
        m_app->config().glRate =  100.0f / (std::max(1.0f,  m_tmp));
        m_app->config().glRate = std::min(100, std::max(4, m_app->config().glRate));
        break;
    }
    case 7:
    {
        m_app->config().show2DMaze =((int)(m_tmp)) % 2;
        break;
    }
    case 8:
    {
        m_app->config().miniMapSize =  (m_tmp) * App::SCREEN_WIDTH / 100.0f;
        m_app->config().miniMapSize = std::min<int>(App::SCREEN_WIDTH/3, std::max<int>(App::SCREEN_WIDTH/60, m_app->config().miniMapSize));
        break;
    }
    case 9:
    {
        m_app->config().rotate2DMaze =((int)(m_tmp )) % 2;
        break;
    }
    }


    return nullptr;
}

void Title::drawMarker(int s, int y, int h){
    if(m_select == s) {
        DrawRectangle( 0, y, App::SCREEN_WIDTH, h*2, GRAY);
        DrawRectangle( 0, y, h*4, h*2, DARKGRAY);
        DrawText("<", h*1.5, y, h*2, WHITE);
        DrawRectangle( App::SCREEN_WIDTH- h*4, y, h*4, h*2, DARKGRAY);
        DrawText(">",  App::SCREEN_WIDTH- h*2.5, y, h*2, WHITE);
    }
}

void Title::draw(){
    ClearBackground(BLACK);

    int x = App::SCREEN_WIDTH/40;
    int y = App::SCREEN_HEIGHT/40;
    int height = App::SCREEN_HEIGHT/vSplit;


    const char* strs[] = {"Auto","Play", "Cheat"};
    int h=height, s=0;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Play Mode : %s", strs[m_app->config().playMode]), height != h ? x+App::SCREEN_WIDTH/4 : x, y,height, WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Map Size : %d", m_app->config().mapSize), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Player Speed: %d", (int)(m_app->config().playerSpeed*500)), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Num Rats : %d", m_app->config().ratsNum), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Rats Speed : %d", (int)(m_app->config().ratsSpeed*500)), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Hedron Freq : %.0f %%", (100.0f / m_app->config().hedronRate)), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("OpenGL Freq : %.0f %%", (100.0f / m_app->config().glRate)), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;

    const char* strs2[]= {"Off", "On"};

    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Show 2D Maze : %s", strs2[m_app->config().show2DMaze ? 1:0]), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("2D Maze Size : %d", int(m_app->config().miniMapSize*100.0f / App::SCREEN_WIDTH)), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;
    drawMarker(s,y,h);

    h=m_select == s++ ? height*2 : height;
    DrawText(FormatText("Flip 2D Maze : %s", strs2[m_app->config().rotate2DMaze ? 1:0]), height != h ? x+App::SCREEN_WIDTH/4 : x,y,height ,WHITE);
    y+=h;

     h=m_select == s++ ? height*2 : height;
    DrawRectangle( 0, y, App::SCREEN_WIDTH, h, h!= height ? SKYBLUE : LIGHTGRAY);
    DrawText("Start", App::SCREEN_WIDTH / 2 - h*1.25 ,y,h ,WHITE);
    y+=h;

}
