#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "winbgim.h"
#include <fstream>
#include <string>

using namespace std;

int w,h,nrOfPlayers,board_size,piece_color=15,board_color,board[100][100],l,player=1,scores[3],best_move[100],aux[100],brow,bcol;
int res_list[100][2]= {{800,800},{1280,720},{1280,1080},{1600,900},{1920,1080}}, res=0;
int di[4]= {-1,0,1,0};
int dj[4]= {0,1,0,-1};

struct tile
{
    int row;
    int col;
} move_list[300][200];

int move_size[1000],current_move=0;

#define FONT SIMPLEX_FONT
#define FONT_SIZE 1
#define MAX_BOARD_SIZE 25
#define MIN_BOARD_SIZE 4

int main();
void startGame();
void startMenu();
void mainMenu();
void optionsMenu();
void boardMenu();
void pieceMenu();
void playGame();
void gameOverMenu();

int isMouseInside(int xi, int yi, int xf, int yf)
{
    int x=mousex();
    int y=mousey();
    return (x>=xi && x<=xf && y>=yi && y<=yf);
}

void buton(int xi, int yi,int xf, int yf, int x, int y)
{
    if (xi<=x&& xf>=x && yi<=y && yf>=y)
    {
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            setfillstyle(1,BLACK);
            bar(xi,yi,xf+1,yi+5);
            readimagefile("Pictures/buton_apasat.jpg",xi,yi+5,xf,yf);
            setbkcolor(COLOR(124,213,84));
            setcolor(WHITE);

        }
        else
        {

            readimagefile("Pictures/buton_highlight.jpg",xi,yi,xf,yf);
            setbkcolor(COLOR(124,213,84));
        }
    }
    else
    {
        readimagefile("Pictures/buton.jpg",xi,yi,xf,yf);
        setbkcolor(COLOR(49,143,5));
    }
}

void resetBoard()
{
    int i,j;
    for(i=0 ; i<=MAX_BOARD_SIZE+2 ; i++)
        for(j=0 ; j<=MAX_BOARD_SIZE+2 ; j++)
            board[i][j]=-1;
}

int ok_board()
{
    int i,j;
    for(i=1 ; i<=board_size ; i++)
        for(j=1 ; j<=board_size ; j++)
            if(board[i][j]==0)
                if(board[i-1][j] && board[i+1][j] && board[i][j-1] && board[i][j+1])
                    return 0;
    return 1;
}

void drawBoard1(int xi, int yi, int xf, int yf)
{
    setfillstyle(SOLID_FILL,board_color);
    bar(xi,yi,xf,yf);
    setbkcolor(BLACK);
    int i,j;
    if(board_size)
        l=(xf-xi)/board_size;
    else
        l=xf-xi;

    setfillstyle(SOLID_FILL,board_color);
    bar(xi,yi,xi+l*board_size,yi+l*board_size);

    for(i=xi ; i<xf-20 ; i=i+l)
        for(j=yi ; j<yf-20; j=j+l)
            rectangle(i,j,i+l,j+l);
}

void drawBoard2(int xi, int yi, int xf, int yf)
{
    int i,j;
    if(board_size)
        l=(xf-xi)/board_size;
    else
        l=xf-xi;
    setbkcolor(BLACK);
    setfillstyle(SOLID_FILL,board_color);
    bar(xi,yi,xi+l*board_size,yi+l*board_size);

    for(i=xi ; i<xf-20 ; i=i+l)
        for(j=yi ; j<yf-20; j=j+l)
        {
            rectangle(i,j,i+l,j+l);
            if(board[(j-100)/l+1][(i-100)/l+1]==-1)
            {
                if(board_color==BLACK)
                    setcolor(RED);
                else
                    setcolor(BLACK);
                line(i+l/5,j+l/5,i+l-l/5,j+l-l/5);
                line(i+l-l/5,j+l/5,i+l/5,j+l-l/5);
                setcolor(WHITE);
            }
        }
}

void drawBoard3(int xi, int yi, int xf, int yf)
{
    int i,j;
    if (board_size)
        l = (xf - xi) / board_size;
    else
        l=xf-xi;
    setbkcolor(BLACK);
    setfillstyle(SOLID_FILL,board_color);
    bar(xi,yi,xi+l*board_size,yi+l*board_size);

    for(i=xi ; i<xf-20 ; i=i+l)
        for(j=yi ; j<yf-20; j=j+l)
        {
            rectangle(i,j,i+l,j+l);
            if(board[(j-100)/l+1][(i-100)/l+1]==-1)
            {
                setfillstyle(SLASH_FILL,RED);
                bar(i+l/5,j+l/5,i+l-l/5,j+l-l/5);
                setfillstyle(SOLID_FILL,WHITE);
            }
        }
}

int game_over()
{
    setbkcolor(BLACK);
    int i,j;
    if(board[1][1]==1)
    {
        if(board[1][2]==1 && (board[1][3]==0 || board[1][3]==3))
            return 0;
        if(board[2][1]==1 && (board[3][1]==0 || board[3][1]==3))
            return 0;
    }
    for(i=2 ; i<=board_size ; i++)
    {
        if(board[i][1]==1)
        {
            if(board[i+1][1]==1 && (board[i+2][1]==0 || board[i+2][1]==3))
                return 0;
            if(board[i-1][1]==1 && (board[i-2][1]==0 || board[i-2][1]==3))
                return 0;
            if(board[i][2]==1 && (board[i][3]==0 || board[i][3]==3))
                return 0;
        }
    }
    for(j=2 ; j<=board_size ; j++)
    {
        if(board[1][j]==1)
        {
            if(board[1][j+1]==1 && (board[1][j+2]==0 || board[1][j+2]==3))
                return 0;
            if(board[1][j-1]==1 && (board[1][j-2]==0 || board[1][j-2]==3))
                return 0;
            if(board[2][j]==1 && (board[3][j]==0 || board[3][j]==3))
                return 0;
        }
    }

    for(i=2 ; i<=board_size ; i++)
        for(j=2 ; j<=board_size ; j++)
            if(board[i][j]==1)
            {
                if(board[i-1][j]==1 && (board[i-2][j]==0 || board[i-2][j]==3))
                    return 0;
                if(board[i+1][j]==1 && (board[i+2][j]==0 || board[i+2][j]==3))
                    return 0;
                if(board[i][j-1]==1 && (board[i][j-2]==0 || board[i][j-2]==3))
                    return 0;
                if(board[i][j+1]==1 && (board[i][j+2]==0 || board[i][j+2]==3))
                    return 0;
            }
    return 1;
}

void finalBoard(int xi, int yi, int xf, int yf)
{
    cleardevice();
    int i,j,fr=l/5;
    if(board_size)
        l=(xf-xi)/board_size;
    else
        l=xf-xi;
    setbkcolor(BLACK);
    setfillstyle(SOLID_FILL,board_color);
    bar(xi,yi,xi+l*board_size,yi+l*board_size);

    if(nrOfPlayers==1)
    {
        if(player==2)
            bgiout<<"Este randul calculatorului";
        else
            bgiout<<"Este randul jucatorului";
    }
    else
        bgiout<<"Este randul jucatorului "<<player;
    outstreamxy(20,20);

    bgiout<<"Scor: "<<scores[1]<<'-'<<scores[2];
    outstreamxy(w-200,20);



    for(i=xi ; i<xf-20 ; i=i+l)
        for(j=yi ; j<yf-20; j=j+l)
        {
            int row=(j-100)/l+1;
            int column=(i-100)/l+1;

            rectangle(i,j,i+l,j+l);

            if(board[row][column]==-1)
            {
                setfillstyle(SLASH_FILL,RED);
                bar(i+l/5,j+l/5,i+l-l/5,j+l-l/5);
                setfillstyle(SOLID_FILL,WHITE);
            }
            if(board[column][row]==1)
            {
                setfillstyle(SOLID_FILL,piece_color);
                fillellipse(100+row*l-l/2, 100+column*l-l/2,l/3,l/3);

            }
            if(board[row][column]==3)
            {
                line(i+fr,j+fr,i+l-fr,j+l-fr);
                line(i+l-fr,j+fr,i+fr,j+l-fr);
                line(i+l-fr,j+l*1/2,i+fr,j+l*1/2);
                line(i+l*1/2,j+fr,i+l*1/2,j+l-fr);
            }
        }

}

void fillBoard(int xi, int yi, int xf, int yf)
{
    int i,j,l=(xf-xi)/board_size;

    setfillstyle(SOLID_FILL,piece_color);
    setbkcolor(BLACK);
    for(i=xi ; i<xf-20 ; i=i+l)
        for(j=yi ; j<yf-20; j=j+l)
        {
            int row=(j-100)/l+1;
            int col=(i-100)/l+1;
            if(board[row][col]!=-1)
            {
                board[row][col]=1;
                fillellipse(i+l/2,j+l/2,l/3,l/3);
            }
        }

}

void emptyBoard(int xi, int yi, int xf, int yf)
{
    int i,j,l=(xf-xi)/board_size;
    setbkcolor(BLACK);
    setfillstyle(SOLID_FILL,board_color);

    for(i=xi ; i<xf-20 ; i=i+l)
        for(j=yi ; j<yf-20; j=j+l)
        {
            int row=(j-100)/l+1;
            int col=(i-100)/l+1;
            if(board[row][col]!=-1)
            {
                board[row][col]=0;
                bar(i+l/8,j+l/8,i+l-l/8,j+l-l/8);
            }
        }
}

void startScreen()
{
    setbkcolor(BLACK);
    cleardevice();

    if(board_size<MIN_BOARD_SIZE)
    {
        setcolor(RED);
        bgiout<<"n < "<<MIN_BOARD_SIZE;
        outstreamxy(w-120,h-90);
        readimagefile("Pictures/buton_blocat.jpg",w-120,h-70,w-20,h-20);
        setbkcolor(COLOR(127,127,127));

        bgiout<<"Inainte";
        outstreamxy(w-100,h-55);

        setbkcolor(BLACK);
    }

    setcolor(WHITE);
    setbkcolor(BLACK);
    bgiout<<"Introduceti dimensiunea tablei ("<<MIN_BOARD_SIZE<<"<= n <= "<<MAX_BOARD_SIZE<<")";
    outstreamxy(20,20);
    bgiout<<"n = "<<board_size;
    setbkcolor(BLACK);
    outstreamxy(20,50);
    drawBoard1(100,100,min(w,h)-100,min(w,h)-100);
}

void puneText (int stg, int sus,char text[100])
{
    int x0=stg,y0=sus;

    unsigned int i=0;
    char a[2];
    while(i<strlen(text))
    {
        a[0]=text[i];
        outtextxy(x0,y0,a);
        x0=x0+textwidth(a);
        delay(5);
        i++;
    }
}

void instructiuniText()
{
    setbkcolor(BLACK);
    cleardevice();
    int L1,L2,x=w/6,y=50;
    fstream f("Text/Instructiuni.txt",ios::in);
    fstream f1("Text/Masa.txt",ios::in);

    setcolor(RED);
    bgiout<<"Gameplay :";
    outstreamxy(w/6-50,20);
    setcolor(WHITE);

    char s1[1024],s2[1024];
    strcpy(s1,"   ");
    while (!f.eof())
    {

        f>>s2;
        L1=strlen(s1);
        L2=strlen(s2); ///calculeaza lunguimile celor 2 siruri.
        if (L1+L2+1 <= (w*2/3)/10) /// verifica daca lungimile adunate sunt mai mari ca 2 treimi din latime.
        {
            strcat(s1," "); /// aduna la s1 pe s2.
            strcat(s1,s2);
        }
        else
        {
            puneText(x,y,s1);
            y=y+25;
            strcpy(s1,s2);
        }

    }

    puneText(x,y,s1);
    setcolor(RED);

    bgiout<<"Masa :";
    outstreamxy(w/6-50,y+30);
    setcolor(WHITE);
    y=y+60;
    strcpy(s1,"   ");
    while (!f1.eof())
    {

        f1>>s2;
        L1=strlen(s1);
        L2=strlen(s2); ///calculeaza lunguimile celor 2 siruri.
        if (L1+L2+1 <= (w*2/3)/10) /// verifica daca lungimile adunate sunt mai mari ca 2 treimi din latime.
        {

            strcat(s1," "); /// aduna la s1 pe s2.
            strcat(s1,s2);
        }
        else
        {
            puneText(x,y,s1);
            y=y+25;
            strcpy(s1,s2);
        }
    }

    puneText(x,y,s1);
    settextstyle(3,0, 5);

    bgiout<<"Spor la joc!";
    outstreamxy(w/2-40,y+40);

    settextstyle(FONT,0, FONT_SIZE);

    int ok=0;
    while(!ok)
    {
        int X=mousex();
        int Y=mousey();
        buton(20,h-70,120,h-20,X,Y);
        bgiout<<"Inapoi";
        outstreamxy(40,h-55);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }
            if(!pressed)
            {
                if(isMouseInside(20,h-70,120,h-20))
                {
                    if(board_color!=piece_color)
                    {
                        ok=1;
                        mainMenu();
                    }
                }
            }
        }
    }
}

void mainMenu()
{

    setbkcolor(BLACK);
    int ok=0;
    scores[1]=0;
    scores[2]=0;

    for(int i=0 ; i<=current_move ; i++)
        move_size[i]=0;

    current_move=0;

    cleardevice();

    readimagefile("Pictures/Titlu.jpg",w/3,h/10,2*w/3,h/5); ///titlu

    while(!ok)
    {
        int x= mousex();
        int y= mousey();
        setcolor(WHITE);
        setbkcolor(BLACK);

        buton(w/3,h/4,2*w/3,h/4+50,x,y); ///un jucator
        bgiout<<"Un jucator";
        outstreamxy(w/2-60,h/4+15);

        buton(w/3,h/4+70,2*w/3,h/4+120,x,y); ///doi jucatori
        bgiout<<"Doi jucatori";
        outstreamxy(w/2-60,h/4+85);

        buton(w/3,h/4+140,2*w/3,h/4+190,x,y); ///optiuni
        bgiout<<"Instructiuni";
        outstreamxy(w/2-60,h/4+155);

        buton(w/3,h/4+210,2*w/3,h/4+260,x,y); ///quit
        bgiout<<"Setari";
        outstreamxy(w/2-40,h/4+225);

        buton(w/3,h/4+280,2*w/3,h/4+330,x,y); ///quit
        bgiout<<"Iesire joc";
        outstreamxy(w/2-50,h/4+295);

        if(ismouseclick(WM_LBUTTONDOWN))
        {

            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }

            if(isMouseInside(w/3,h/4,2*w/3,h/4+50))
            {
                if(!pressed)
                {
                    ok=1;
                    nrOfPlayers=1;
                    startMenu();
                }
            }

            else if(isMouseInside(w/3,h/4+70,2*w/3,h/4+120))
            {
                if(!pressed)
                {
                    ok=1;
                    nrOfPlayers=2;
                    startMenu();
                }
            }
            else if(isMouseInside(w/3,h/4+140,2*w/3,h/4+190))
            {
                if(!pressed)
                {
                    ok=1;
                    instructiuniText();
                }
            }
            else if(isMouseInside(w/3,h/4+210,2*w/3,h/4+260))
            {
                if(!pressed)
                {
                    ok=1;
                    optionsMenu();
                }
            }
            else if(isMouseInside(w/3,h/4+280,2*w/3,h/4+330))
            {
                if(!pressed)
                {
                    ok=1;
                    closegraph();

                }
            }
        }
    }
}

void startMenu()
{
    setbkcolor(BLACK);
    int ok=0;
    char t;
    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);

    startScreen();


    while(!ok)
    {

        int x=mousex();
        int y=mousey();
        setbkcolor(BLACK);
        if(kbhit())
        {
            t=getch();
            if(t>='0' && t<='9' && board_size*10+t-'0'<=MAX_BOARD_SIZE)
                board_size=board_size*10+t-'0';
            else if(t==127 || t==8)
                board_size=board_size/10;
            startScreen();
        }

        buton(20,h-70,120,h-20,x,y);
        bgiout<<"Inapoi";
        outstreamxy(40,h-55);

        if (board_size>=MIN_BOARD_SIZE)
        {
            buton(w-120,h-70,w-20,h-20,x,y);
            bgiout<<"Inainte";
            outstreamxy(w-100,h-55);
            setbkcolor(BLACK);
        }

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }
            if(!pressed)
            {
                if(isMouseInside(20,h-70,120,h-20))
                {
                    ok=1;
                    mainMenu();
                }
                else
                {

                    if( board_size>=MIN_BOARD_SIZE)
                    {


                        if (isMouseInside(w-120,h-70,w-20,h-20) )
                        {

                            ok=1;
                            resetBoard();
                            for(int i=1 ; i<=board_size ; i++)
                                for(int j=1 ; j<=board_size ; j++)
                                    board[i][j]=0;
                            boardMenu();
                        }
                    }

                }
            }
        }
    }
}

void editBoard()
{
    int x=mousex();
    int y=mousey();
    int l=(min(w,h)-200)/board_size;
    int row=(x-100)/l+1;
    int column=(y-100)/l+1;
    x=100+(row-1)*l;
    y=100+(column-1)*l;
    int fr=l/5;
    if(board[column][row]!=-1)
    {
        if(board_color==BLACK)
            setcolor(RED);
        else
            setcolor(BLACK);
        line(x+fr,y+fr,x+l-fr,y+l-fr);
        line(x+l-fr,y+fr,x+fr,y+l-fr);
        board[column][row]=-1;
    }
    else if(board[column][row]==-1)
    {
        setfillstyle(SOLID_FILL,board_color);
        bar(x+fr-1,y+fr-1,x+l-fr+1,y+l-fr+1);
        board[column][row]=0;
    }
}

void boardMenu()
{
    setbkcolor(BLACK);
    cleardevice();
    int ok=0;

    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);
    bgiout<<"Selectati forma tablei de joc";
    outstreamxy(20,20);
    drawBoard2(100,100,min(w,h)-100,min(w,h)-100);

    while(!ok)
    {
        int x=mousex();
        int y=mousey();
        setcolor(WHITE);
        setfillstyle(SOLID_FILL,WHITE);
        buton(w-120,h-70,w-20,h-20,x,y);
        bgiout<<"Inainte";
        outstreamxy(w-100,h-55);

        buton(20,h-70,120,h-20,x,y);
        bgiout<<"Inapoi";
        outstreamxy(40,h-55);

        setbkcolor(BLACK);

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }
            if(!pressed)
            {

                if(isMouseInside(20,h-70,120,h-20))
                {
                    ok=1;
                    startMenu();
                }
                else if(isMouseInside(w-120,h-70,w-20,h-20))
                {
                    if(ok_board())
                    {
                        ok=1;
                        pieceMenu();
                    }
                    else
                    {
                        setcolor(RED);
                        bgiout<<"Exista patratele izolate";
                        outstreamxy(w-250,h-90);
                        setcolor(WHITE);
                    }
                }
                else if(isMouseInside(100,100,min(w,h)-100,min(w,h)-100))
                    editBoard();
            }
        }

    }

}

void editPieces()
{
    int x=mousex();
    int y=mousey();
    int l=(min(w,h)-200)/board_size;
    int line1=(x-100)/l+1;
    int column=(y-100)/l+1;
    if(board[column][line1]!=-1)
    {
        if(board[column][line1]==0)
        {
            setfillstyle(SOLID_FILL,piece_color);
            fillellipse(100+line1*l-l/2, 100+column*l-l/2,l/3,l/3);

            board[column][line1]=1;
        }
        else if(board[column][line1]==1)
        {
            setfillstyle(SOLID_FILL,board_color);
            bar(100+(line1-1)*l+l/8,100+(column-1)*l+l/8,100+line1*l-l/8,100+column*l-l/8);
            board[column][line1]=0;
        }
    }
}

int verify()
{
    int ok=0,nr0=0;
    for ( int line=1; line<=board_size; line++)
        for (int col=1; col<=board_size; col++)
        {
            if(board[line][col]==1)
            {
                if(!ok)
                    if(board[line+1][col]==1 || board[line-1][col]==1 || board[line][col+1]==1 || board[line][col-1]==1)
                        ok=1;
            }
            else if(board[line][col]==0)
                nr0++;
        }
    if(ok && nr0)
        return 1;
    return 0;
}

void pieceMenu()
{
    setbkcolor(BLACK);

    int ok=0;
    cleardevice();

    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);

    bgiout<<"Selectati pozitia de inceput:";
    outstreamxy(20,20);
    bgiout<<"(Cel putin o casuta trebuie sa fie goala)";
    outstreamxy(20,40);
    bgiout<<"(Cel putin o piesa trebuie adaugata)";
    outstreamxy(20,60);

    drawBoard3(100,100,min(w,h)-100,min(w,h)-100);

    setcolor(WHITE);
    setbkcolor(COLOR(127,127,127));
    readimagefile("Pictures/buton_blocat.jpg",w-120,h-70,w-20,h-20);
    bgiout<<"Inainte";
    outstreamxy(w-100,h-55);
    int ok_pieces=0;

    while(!ok)
    {
        int x=mousex();
        int y=mousey();

        buton(20,h-70,120,h-20,x,y);
        bgiout<<"Inapoi";
        outstreamxy(40,h-55);

        buton(w/4,h-70,w/2-50,h-20,x,y);
        bgiout<<"Umple tabla";
        outstreamxy(w/4+15,h-55);

        buton(w/2+50,h-70,3*w/4,h-20,x,y);
        bgiout<<"Goleste tabla";
        outstreamxy(w/2+65,h-55);
        setbkcolor(BLACK);

        if (ok_pieces)
        {
            buton(w-120,h-70,w-20,h-20,x,y);
            bgiout<<"Inainte";
            outstreamxy(w-100,h-55);
        }
        else
        {
            setcolor(WHITE);
            setbkcolor(COLOR(127,127,127));
            readimagefile("Pictures/buton_blocat.jpg",w-120,h-70,w-20,h-20);
            bgiout<<"Inainte";
            outstreamxy(w-100,h-55);
            setbkcolor(BLACK);
        }

        if(ismouseclick(WM_LBUTTONDOWN))
        {

            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }

            if(!pressed)
            {
                if(isMouseInside(20,h-70,120,h-20))
                {
                    ok=1;
                    boardMenu();
                }
                else if(isMouseInside(w-120,h-70,w-20,h-20))
                {
                    if(ok_pieces)
                    {
                        ok=1;
                        startGame();
                    }
                }
                else if(isMouseInside(100,100,min(w,h)-100,min(w,h)-100))
                {
                    editPieces();
                    if(verify())
                        ok_pieces=1;
                    else
                        ok_pieces=0;
                }
                else if(isMouseInside(w/4,h-70,w/2-50,h-20))
                {
                    fillBoard(100,100,min(w,h)-100,min(w,h)-100);
                    ok_pieces=0;
                }
                else if(isMouseInside(w/2+50,h-70,3*w/4,h-20))
                {
                    emptyBoard(100,100,min(w,h)-100,min(w,h)-100);

                    if(verify())
                        ok_pieces=1;
                    else
                        ok_pieces=0;
                }
            }
        }
    }
}

int mutarePosibila(int row,int col)
{
    if((board[col+2][row]==0 && board[col+1][row]==1) || (board[col-2][row]==0 && board[col-1][row]==1) || (board[col][row+2]==0 && board[col][row+1]==1) || (board[col][row-2]==0 && board[col][row-1]==1))
        return 1;
    else
        return 0;
}

void highlight()
{
    int x=mousex();
    int y=mousey();
    int l=(min(w,h)-200)/board_size;
    int row=(x-100)/l+1;
    int column=(y-100)/l+1;

    if(board[column][row]==1)
    {
        if (board[column+2][row]==0 && board[column+1][row]==1)
            board[column+2][row]=3;

        if (board[column-2][row]==0 && board[column-1][row]==1)
            board[column-2][row]=3;

        if (board[column][row+2]==0 && board[column][row+1]==1)
            board[column][row+2]=3;

        if (board[column][row-2]==0 && board[column][row-1]==1)
            board[column][row-2]=3;
    }
}

void findBestMove(int step, int row, int col, int &bestSize, int &ok)
{
    setbkcolor(BLACK);
    for(int con=1 ; con<=4 ; con++)
    {
        int i=con-1;
        if(row+2*di[i]>0 && col+2*dj[i]>0)
            if(board[row+di[i]][col+dj[i]]==1 && board[row+2*di[i]][col+2*dj[i]]==0)
            {
                board[row][col]=0;
                board[row+di[i]][col+dj[i]]=0;
                board[row+2*di[i]][col+2*dj[i]]=1;

                aux[step]=con;

                if(step>bestSize)
                {
                    for(int j=0 ; j<=step ; j++)
                        best_move[j]=aux[j];
                    bestSize=step;
                    ok=1;
                }

                findBestMove(step+1,row+2*di[i],col+2*dj[i],bestSize,ok);

                aux[step]=0;

                board[row][col]=1;
                board[row+di[i]][col+dj[i]]=1;
                board[row+2*di[i]][col+2*dj[i]]=0;
            }
    }
}

void makeBestMove()
{
    setbkcolor(BLACK);
    int row,col,bestSize=-1,i,j,ok=0;
    for(row=1 ; row<=board_size ; row++)
        for(col=1 ; col<=board_size ; col++)
            if(board[row][col]==1)
            {
                ok=0;
                findBestMove(0,row,col,bestSize,ok);
                if(ok)
                {
                    brow=row;
                    bcol=col;
                }
            }

    for(i=0 ; i<=bestSize ; i++)
    {
        j=best_move[i]-1;

        board[brow][bcol]=0;
        board[brow+di[j]][bcol+dj[j]]=0;
        board[brow+2*di[j]][bcol+2*dj[j]]=1;

        move_list[current_move][++move_size[current_move]].row=brow;
        move_list[current_move][move_size[current_move]].col=bcol;

        move_list[current_move][++move_size[current_move]].row=brow+di[j];
        move_list[current_move][move_size[current_move]].col=bcol+dj[j];

        move_list[current_move][++move_size[current_move]].row=brow+2*di[j];
        move_list[current_move][move_size[current_move]].col=bcol+2*dj[j];

        brow=brow+2*di[j];
        bcol=bcol+2*dj[j];

        scores[2]++;

        delay(750);
        setbkcolor(BLACK);
        finalBoard(100,100,min(w,h)-100,min(w,h)-100);
    }
}

void startGame()
{
    setbkcolor(BLACK);
    cleardevice();
    finalBoard(100,100,min(w,h)-100,min(w,h)-100);
    int row,column,rowi,columni,piece_moved=0;
    int ok=0,is_highlight=0;
    while (ok!=1)
    {
        int x=mousex();
        int y=mousey();
        buton(w-120,h-70,w-20,h-20,x,y);
        bgiout<<"Undo";
        outstreamxy(w-100,h-55);
        setbkcolor(BLACK);

        if(game_over())
            ok=1;
not_clicked:
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }
            if(!pressed)
            {
                if(isMouseInside(100,100,min(w,h)-100,min(w,h)-100))
                {

                    x=mousex();
                    y=mousey();

                    row=(y-100)/l+1;
                    column=(x-100)/l+1;

                    if(piece_moved && board[row][column]!=3)
                        goto not_clicked;

                    if(is_highlight==0 && board[row][column]==1)
                    {
continue_move:
                        rowi=row;
                        columni=column;
                        highlight();
                        is_highlight=1;
                        setbkcolor(BLACK);
                        finalBoard(100,100,min(w,h)-100,min(w,h)-100);
                    }
                    else if(is_highlight==1)
                    {
                        if(board[row][column]==3)
                        {
                            board[row][column]=1;
                            board[(row+rowi)/2][(column+columni)/2]=0;
                            board[rowi][columni]=0;

                            move_list[current_move][++move_size[current_move]].row=rowi;
                            move_list[current_move][move_size[current_move]].col=columni;

                            move_list[current_move][++move_size[current_move]].row=(row+rowi)/2;
                            move_list[current_move][move_size[current_move]].col=(column+columni)/2;

                            move_list[current_move][++move_size[current_move]].row=row;
                            move_list[current_move][move_size[current_move]].col=column;

                            piece_moved=1;
                            if(nrOfPlayers==2)
                                scores[player]++;
                            else
                                scores[1]++;
                        }

                        if(board[rowi+2][columni]==3)
                            board[rowi+2][columni]=0;

                        if(board[rowi-2][columni]==3)
                            board[rowi-2][columni]=0;

                        if(board[rowi][columni+2]==3)
                            board[rowi][columni+2]=0;

                        if(board[rowi][columni-2]==3)
                            board[rowi][columni-2]=0;
                        setbkcolor(BLACK);
                        finalBoard(100,100,min(w,h)-100,min(w,h)-100);

                        if(mutarePosibila(column,row))
                        {
                            goto continue_move;
                        }
                        else if(piece_moved)
                        {
                            current_move++;

                            is_highlight=0;
                            player=3-player;

                            if(nrOfPlayers==1)
                            {
                                if(player==2)
                                    bgiout<<"Este randul calculatorului";
                                else
                                    bgiout<<"Este randul jucatorului";
                            }
                            else
                                bgiout<<"Este randul jucatorului "<<player;
                            outstreamxy(20,20);

                            if(nrOfPlayers==1)
                            {

                                makeBestMove();

                                current_move++;

                                player=3-player;

                                setfillstyle(SOLID_FILL,BLACK);
                                bar(20,20,w/2,50);

                                bgiout<<"Este randul jucatorului";
                                outstreamxy(20,20);
                            }
                            piece_moved=0;
                        }
                    }
                }
                else if(isMouseInside(w-120,h-70,w-20,h-20)) ///Undo
                {
                    if(current_move>0 && !is_highlight)
                    {
                        int a;

                        current_move--;

                        for(a=move_size[current_move] ; a>0 ; a--)
                        {
                            board[move_list[current_move][a].row][move_list[current_move][a].col]=1-board[move_list[current_move][a].row][move_list[current_move][a].col];
                        }
                        player=3-player;
                        scores[player]=scores[player]-move_size[current_move]/3;
                        move_size[current_move]=0;

                        if(nrOfPlayers==1)
                        {
                            current_move--;

                            for(a=move_size[current_move] ; a>0 ; a--)
                            {
                                board[move_list[current_move][a].row][move_list[current_move][a].col]=1-board[move_list[current_move][a].row][move_list[current_move][a].col];
                            }
                            player=3-player;
                            scores[player]=scores[player]-move_size[current_move]/3;
                            move_size[current_move]=0;
                        }
                        setbkcolor(BLACK);
                        finalBoard(100,100,min(w,h)-100,min(w,h)-100);
                    }
                }
            }
            setbkcolor(BLACK);
        }
    }
    gameOverMenu();
}

void gameOverMenu()
{
    setbkcolor(BLACK);
    int ok=0;
    cleardevice();
    finalBoard(100,100,min(w,h)-100,min(w,h)-100);
    setfillstyle(SOLID_FILL,BLACK);
    bar(0,0,w,90);
    bar(w-200,h-95,w,h);
    if(scores[1]==scores[2])
    {
        bgiout<<"Este remiza!";
        outstreamxy(20,20);
    }
    else
    {
        if(nrOfPlayers==2)
        {
            bgiout<<"Jucatorul "<<(scores[1]>scores[2] ? 1 : 2)<<" a castigat!";
            outstreamxy(20,20);
        }
        else
        {
            if(scores[1]>scores[2])
                bgiout<<"Felicitari! Ai castigat";
            else
                bgiout<<"Calculatorul a castigat!";
            outstreamxy(20,20);
        }
    }
    bgiout<<"Scor final: "<<scores[1]<<'-'<<scores[2];
    outstreamxy(20,80);

    while(!ok)
    {
        int x=mousex();
        int y=mousey();
        buton(20,h-70,150,h-20,x,y);
        bgiout<<"Joc nou";
        outstreamxy(40,h-55);

        if(ismouseclick(WM_LBUTTONDOWN))
        {
            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }
            if(!pressed)
            {
                if(isMouseInside(20,h-70,150,h-20))
                {
                    ok=1;
                    mainMenu();
                }
            }
        }
    }
}

void optionsMenu()
{
    setbkcolor(BLACK);
    int ok=0;

    cleardevice();

    setcolor(WHITE);
    setfillstyle(SOLID_FILL,WHITE);

    bgiout<<"Culoarea pieselor:";
    outstreamxy(20,20);

    setfillstyle(SOLID_FILL,piece_color);
    bar(260,20,290,50);
    rectangle(260,20,290,50);

    setbkcolor(BLACK);
    bgiout<<"Rezolutia ecranului:";
    outstreamxy(20,100);

    bgiout<<res_list[res][0]<<'x'<<res_list[res][1];
    outstreamxy(290,100);

    setbkcolor(BLACK);
    bgiout<<"Culoarea tablei:";
    outstreamxy(20,180);

    setfillstyle(SOLID_FILL,board_color);
    bar(260,180,290,210);
    rectangle(260,180,290,210);


    while(!ok)
    {
        int x=mousex();
        int y=mousey();


        if (220<=x&& 250>=x && 20<=y && 50>=y)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                readimagefile("Pictures/Left_apasat.jpg",220,20,250,50);
                setcolor(WHITE);
            }
            else
            {
                readimagefile("Pictures/Left_highlight.jpg",220,20,250,50);
            }
        }
        else
        {
            readimagefile("Pictures/Left.jpg",220,20,250,50);
        }

        if (300<=x&& 330>=x && 20<=y && 50>=y)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                readimagefile("Pictures/Right_apasat.jpg",300,20,330,50);
                setcolor(WHITE);
            }
            else
            {
                readimagefile("Pictures/Right_highlight.jpg",300,20,330,50);
            }
        }
        else
        {
            readimagefile("Pictures/Right.jpg",300,20,330,50);
        }

        if (250<=x&& 280>=x && 100<=y && 130>=y)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                readimagefile("Pictures/Left_apasat.jpg",250, 100,280,130);
                setcolor(WHITE);
            }
            else
            {
                readimagefile("Pictures/Left_highlight.jpg",250, 100,280,130);
            }
        }
        else
        {
            readimagefile("Pictures/Left.jpg",250, 100,280,130);
        }

        if (430<=x&& 460>=x && 100<=y && 130>=y)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                readimagefile("Pictures/Right_apasat.jpg",430,100,460,130);
                setcolor(WHITE);
            }
            else
            {
                readimagefile("Pictures/Right_highlight.jpg",430,100,460,130);
            }
        }
        else
        {
            readimagefile("Pictures/Right.jpg",430,100,460,130);
        }

        if (220<=x&& 250>=x && 180<=y && 210>=y)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                readimagefile("Pictures/Left_apasat.jpg",220,180,250,210);
                setcolor(WHITE);
            }
            else
            {
                readimagefile("Pictures/Left_highlight.jpg",220,180,250,210);
            }
        }
        else
        {
            readimagefile("Pictures/Left.jpg",220,180,250,210);
        }

        if (300<=x&& 330>=x && 180<=y && 210>=y)
        {
            if(ismouseclick(WM_LBUTTONDOWN))
            {
                readimagefile("Pictures/Right_apasat.jpg",300,180,330,210);
                setcolor(WHITE);
            }
            else
            {
                readimagefile("Pictures/Right_highlight.jpg",300,180,330,210);
            }
        }
        else
        {
            readimagefile("Pictures/Right.jpg",300,180,330,210);
        }

        buton(20,h-70,120,h-20,x,y);
        bgiout<<"Inapoi";
        outstreamxy(40,h-55);

        buton(480,100,590,130,x,y);
        bgiout<<"Aplica";
        outstreamxy(500,105);
        setbkcolor(BLACK);
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            bool pressed=true;

            if(ismouseclick(WM_LBUTTONUP))
            {
                clearmouseclick(WM_LBUTTONDOWN);
                clearmouseclick(WM_LBUTTONUP);
                pressed=false;
            }
            if(!pressed)
            {


                if(isMouseInside(20,h-70,120,h-20))
                {
                    if(board_color!=piece_color)
                    {
                        ok=1;
                        mainMenu();
                    }
                    else
                    {
same_color:
                        setcolor(RED);
                        bgiout<<"Tabla si piesele au aceeasi culoare";
                        outstreamxy(20,h-90);
                        setcolor(WHITE);
                    }
                }
                else if(isMouseInside(220,20,250,50))
                {
                    if(piece_color==1)
                        piece_color=15;
                    else
                        piece_color--;
                    setfillstyle(SOLID_FILL,piece_color);
                    bar(260,20,290,50);
                    rectangle(260,20,290,50);
                }
                else if(isMouseInside(300,20,330,50))
                {
                    if(piece_color==15)
                        piece_color=1;
                    else
                        piece_color++;
                    setfillstyle(SOLID_FILL,piece_color);
                    bar(260,20,290,50);
                    rectangle(260,20,290,50);
                }
                else if(isMouseInside(220,180,250,210))
                {
                    if(board_color==0)
                        board_color=14;
                    else
                        board_color--;
                    setfillstyle(SOLID_FILL,board_color);
                    bar(260,180,290,210);
                    rectangle(260,180,290,210);
                }
                else if(isMouseInside(300,180,330,210))
                {
                    if(board_color==14)
                        board_color=0;
                    else
                        board_color++;
                    setfillstyle(SOLID_FILL,board_color);
                    bar(260,180,290,210);
                    rectangle(260,180,290,210);
                }
                else if(isMouseInside(250,100,280,130))
                {
                    if(res>0)
                        res--;
                    else
                        res=4;
                    setfillstyle(SOLID_FILL,0);
                    bar(281,100,429,130);
                    bgiout<<res_list[res][0]<<'x'<<res_list[res][1];
                    outstreamxy(290,100);
                }
                else if(isMouseInside(430,100,460,130))
                {
                    if(res_list[res+1][0]!=0)
                        res++;
                    else
                        res=0;
                    setfillstyle(SOLID_FILL,0);
                    bar(281,100,429,130);
                    bgiout<<res_list[res][0]<<'x'<<res_list[res][1];
                    outstreamxy(290,100);
                }
                else if(isMouseInside(480,100,590,130))
                {
                    if(board_color==piece_color)
                        goto same_color;
                    ok=1;
                    closegraph();
                    main();
                }
            }
        }
    }
}

int main()
{
    w=res_list[res][0];
    h=res_list[res][1];

    initwindow(w,h,"Solitar");
    settextstyle(FONT,0,FONT_SIZE);

    mainMenu();

    return 0;
}
