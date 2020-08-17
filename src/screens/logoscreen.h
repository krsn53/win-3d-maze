#pragma once

#include "screen.h"

class LogoScreen : public Screen
{
public:
    LogoScreen();

    void init() override;
    Screen* update() override;
    void draw() override;

private:
    int framesCounter = 0;
    int finishScreen = 0;

    int logoPositionX = 0;
    int logoPositionY = 0;

    int lettersCount = 0;

    int topSideRecWidth = 0;
    int leftSideRecHeight = 0;

    int bottomSideRecWidth = 0;
    int rightSideRecHeight = 0;

    char raylib[8] = { 0 };     // raylib text array, max 8 letters
    int state = 0;              // Tracking animation states (State Machine)
    float alpha = 1.0f;         // Useful for fading
};

