
#include <rlgl.h>

#include "./screens/maze.h"
#include "./screens/title.h"
#include "app.h"
#include <raylib.h>
#include <memory.h>


App::App() :
    m_screen(nullptr)
{
    m_config.playMode= AUTO_MODE;
    m_config.ratsNum = 1;
    m_config.mapSize= 10;
    m_config.ratsSpeed = m_config.playerSpeed = 0.02f;
    m_config.miniMapSize= App::SCREEN_WIDTH/15;
    m_config.show2DMaze = false;
    m_config.rotate2DMaze= false;
    m_config.hedronRate = 5;
    m_config.glRate = 5;
}


// Update and draw game frame
void App::update()
{

    Screen* next = m_screen->update();
    if(next) {
        m_screen->remove();
        delete m_screen;
        m_screen = next;
        next->initialize(this);
    }

    BeginDrawing();
        m_screen->draw();

        DrawFPS(10, 10);
        draw_cursor();
    EndDrawing();
    //----------------------------------------------------------------------------------
}

static void updateFrame(void* arg){
    auto app = static_cast<App*>(arg);
    app->update();
}

int App::run(int argc, char** argv){

    // Initialization (Note windowTitle is unused on Android)
    //---------------------------------------------------------
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze3D");
    HideCursor();

    m_screen = new Title();
    m_screen->initialize(this);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(updateFrame, this, 0, 1);
#else
    SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        update();
    }
#endif
    delete m_screen;


    CloseWindow();          // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void App::draw_cursor(){
    if(!IsCursorOnScreen()) return ;

    const Vector2 mouse = GetMousePosition();

    const float width = 40;
    const float width_s = width*0.7071;
    Vector2 p1 = mouse;
    Vector2 p2 = {mouse.x, mouse.y + width};
    Vector2 p3 = {mouse.x + width*0.3827f*0.75f, mouse.y + width*0.9238f*0.75f};
    Vector2 p4 = {mouse.x + width_s, mouse.y + width_s};

    DrawTriangle(p1, p2, p3, BROWN);
    DrawTriangle(p1, p3, p4, RAYWHITE);
}
