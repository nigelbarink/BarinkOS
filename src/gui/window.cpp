#include "window.h"

int Window::getWidth(){
    return this->rect.width;
}

int Window::getHeight(){
    return this->rect.height;
}

void Window::setWidth(int& width){
    this->rect.width = width;
}
void Window::setHeight(int& height){
    this->rect.height = height;
}

int Window::getX(){
    return this->rect.x;
}

int Window::getY(){
    return this->rect.y;
}

Window::Window (Rect& rect , uint32_t colour){
    this->rect = rect;
    this->Background_colour = colour;
}

void Window::draw(){
    drawRect(this->getX() , this->getY() , this->getWidth() , this->getHeight() ,this->Background_colour );
}
