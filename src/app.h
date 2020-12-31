#pragma once

#include<stdlib.h>
#include <raylib.h>
#include <math.h>
#include <vector>

#include <memory>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

class Screen;

typedef enum PlayMode{
    AUTO_MODE,
    PLAY_MODE,
    CHEAT_MODE,
    NUM_PLAY_MODES,
}PlayMode;

typedef struct Config {
    int playMode;
    int mapSize;
    float playerSpeed;
    int ratsNum;
    float ratsSpeed;
    int hedronRate;
    int glRate;
    bool show2DMaze;
    int miniMapSize;
    bool rotate2DMaze;
}Config;

class App
{
public:
    static constexpr const uint32_t SCREEN_WIDTH = 1280;
    static constexpr const uint32_t SCREEN_HEIGHT = 720;

    App();

    int run(int argc, char **argv);
    void update();

    inline Config& config()  { return m_config; }

    void draw_cursor();
private:
    Screen *m_screen;
    Config m_config;
};

