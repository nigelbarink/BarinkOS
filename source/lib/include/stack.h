#pragma once
#include "../../kernel/memory/KernelHeap.h"
#include <stdint.h>

template <typename T>
class Stack {
    public:
        inline Stack() {
                elements = (T[MAX_STACK_SIZE]) malloc(MAX_STACK_SIZE * sizeof(T));
                num = 0;

        }

        inline void  Push(T element){
            num++;
            if(num > MAX_STACK_SIZE)
                grow();

            element[num] = element;
        }

     
        inline T Pop()
        {
            T temp = elements[num];
            num --;

            return temp;
        }

        inline bool isEmpty()
        {
            return num == 0;
        }

        inline bool isFull()
        {
                return num == MAX_STACK_SIZE;
        }

      
        inline int count()
        {
                return num;
        }

        inline ~Stack()
        {
            free(elements);
        }


    private:
        unsigned int MAX_STACK_SIZE;
        T[MAX_STACK_SIZE] elements;
        unsigned int num;

        inline void grow (){
            MAX_STACK_SIZE = MAX_STACK_SIZE + (int)(MAX_STACK_SIZE / 4);
            T[] new_elements =(T[MAX_STACK_SIZE]) malloc(MAX_STACK_SIZE * sizeof(T));

            for ( int i = 0; i < num ; i++){
                new_elements[i] = elements[i];
            }
            free(elements);
            elements = new_elements;

        }



};

