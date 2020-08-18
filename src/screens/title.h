#pragma once

#include "./screen.h"

class Title : public Screen
{
public:
    Title();
    void init() override;
    Screen* update() override;
    void draw() override;


private:
    void setSelect(int s);
    void drawMarker(int s,int y, int h);
    static constexpr const int vSplit = 12;

    Rectangle m_start;
    float m_tmp;
    int m_select;
    int m_keyTime;
};

