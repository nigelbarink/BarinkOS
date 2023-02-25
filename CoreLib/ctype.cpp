//
// Created by nigel on 25/02/23.
//
#include "ctype.h"

int isupper (int ch){
    if( ch >= 'A' && ch <= 'Z'){
        return 1;
    }
    return 0;
}
int islower (int ch){
    if(ch >= 'a' && ch <= 'z'){
        return 1;
    }
    return 0;
}

int isalpha (int ch) {
    if(isupper(ch)){
        return 1;
    }
    if(islower(ch)){
        return 1;
    }
    return 0;
}
int isdigit (int ch){
    if(ch >= '0' && ch <= '9'){
        return 1;
    }
    return 0;
}

int isxdigit (int ch){
    if(isdigit(ch)){
        return 1;
    }

    if( ch >= 'a' && ch <= 'f'){
        return 1;
    }

    if( ch >= 'A' && ch <= 'F'){
        return 1;
    }
    return 0;
}

int iscntrl (int ch){
    if(ch >= 0x00 && ch <= 0x1f )
        return 1;
    if(ch == 0x7f)
        return 1;
    return 0;
}


int isgraph (int ch){
    if(isdigit(ch))
        return 1;
    if(isupper(ch))
        return 1;
    if(islower(ch))
        return 1;
    if(ispunct(ch))
        return 1;
    return 0;
}

int isspace(int ch){
    if (ch == 0x20)
        return 1;
    if(ch == 0x0c)
        return 1;
    if(ch == 0x0a)
        return 1;
    if(ch == 0x0d)
        return 1;
    if(ch == 0x09)
        return 1;
    if(ch == 0x0b)
        return 1;

    return 0;
}

int isblank (int ch){
    if( ch == 0x20 || ch == 0x09)
        return 1;
    return 0;
}

int ispunct(int ch){
    if(ch >= '!' && ch <= '~')
        return 1;
    return 0;
}

int isprint (int ch){
    if (isdigit(ch))
        return 1;
    if(isupper(ch))
        return 1;
    if(islower(ch))
        return 1;
    if(ispunct(ch))
        return 1;
    if(isspace(ch))
        return 1;

}


int tolower(int ch){

    if(islower(ch)) return ch;
    int diff = 'a' - 'A';
    return ch + diff;
}

int toupper(int ch){
    if(isupper(ch)) return ch;
    int diff = 'a' - 'A';
    return ch - diff;
}



int isalnum (int ch){
    if(isdigit(ch)){
        return 1;
    }

   if(isalpha(ch)){
       return 1;
   }

    return 0;

}