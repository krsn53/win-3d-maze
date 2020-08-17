
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
    m_config.mapSize= 15;
    m_config.ratsSpeed = m_config.playerSpeed = 0.02f;
    m_config.miniMapSize= App::SCREEN_WIDTH/15;
    m_config.show2DMaze = false;
    m_config.rotate2DMaze= false;
    m_config.hedronRate = 50;
    m_config.glRate = 100;
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

