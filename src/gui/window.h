#pragma once
#include "../kernel/drivers/vesa/vesa.h"
#include "Widget.h"

struct Rect {
    int width;
    int height;
    int x;
    int y;

};



class Window : Widget{

public:
    int getX();
    int getY();

    int getWidth();
    int getHeight();
    void setWidth(int&);
    void setHeight(int&);

    Window (Rect& rect , uint32_t colour);
    void draw();


private:
    Rect rect;
    uint32_t Background_colour;




};
