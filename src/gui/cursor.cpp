#include "cursor.h"

    void Cursor::draw(){
        for(int i = 0; i <  this->width; i++){
            for(int j = 0; j <  this->height; j++){
                if(this->bitmap[j * this->width + i] == 1 ){
                        putPixel(i + this->x,j + this->y, 0xFF000000);
                }
            }
        }
    }


    Cursor::Cursor(int x, int y){
        this->x = x;
        this->y = y;
    }