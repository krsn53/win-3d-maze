#pragma once

#include <raylib.h>

class App;

class Screen
{
public:
    Screen();
    virtual ~Screen();

    inline void initialize(App* app) { m_app = app; init();  }

    virtual Screen* update() { return nullptr; }
    virtual void draw() {}
    virtual void remove() {}
protected:

    virtual void init() { }
    App* m_app;
};

