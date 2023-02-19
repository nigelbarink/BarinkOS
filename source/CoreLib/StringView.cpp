//
// Created by nigel on 19/02/23.
//
#include "StringView.h"

StringView::StringView(String string, unsigned int start, unsigned int end)
        : String(string), begin(start), end(end)
{

}

char* StringView::str(){
   //TODO: Not implemented
}