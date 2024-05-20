#include <iostream>
#include <windows.h>

#define WIDTH 70
#define WEIGHT 35

void gotoxy(int x, int y){

    COORD coord;
    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}


void setConsoleSize(int cols, int rows){

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if(hOut == INVALID_HANDLE_VALUE)
        return; 

    SMALL_RECT r;

    r.Left = 0;
    r.Top = 0;
    r.Right = cols -1;
    r.Bottom = rows -1;
    SetConsoleWindowInfo(hOut, TRUE, &r);

    COORD c;

    c.X = cols;
    c.Y = rows;

    SetConsoleScreenBufferSize(hOut, c);

}



int main(){
    
    system("cls");
    setConsoleSize(78,30);
    std::cout << "Dimession is ";

    gotoxy(30,0);
    std::cout << "Hello World";

    return 0;
}
