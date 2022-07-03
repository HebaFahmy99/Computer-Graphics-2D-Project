#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <bits/stdc++.h>
#include <iostream>
#include <windowsx.h>
#include<vector>
#include <string>
#include <commdlg.h>

using namespace std;
#define Clear_Menu 0
#define FILE_MENU_SAVE 1
#define FILE_MENU_LOAD 2


#define Color_Menue_Red 3
#define Color_Menue_Green 4
#define Color_Menue_Blue 5
#define Color_Menue_Yellow 6
#define Color_Menue_Black 7

#define LINE_MENU_PARAMETRIC 8
#define LINE_MENU_MIDPOINT 9
#define LINE_MENU_DDA 10

#define CIRCLE_MENU_DIRECT 11
#define CIRCLE_MENU_POLAR_ITERATIVE 12
#define CIRCLE_MENU_POLAR 13
#define CIRCLE_MENU_MIDPOINT 14
#define CIRCLE_MENU_MODIFIED_MIDPOINT 15

#define FILLING_FIRST_QUARTER 16
#define FILLING_SECOND_QUARTER 17
#define FILLING_THIRD_QUARTER 18
#define FILLING_FOURTH_QUARTER 19

#define ELIPSE_MENU_DIRECT 20
#define ELIPSE_MENU_POLAR 21

#define CLIPPING_MENU_POINT 22
#define CLIPPING_MENU_LINE 23

#define Red RGB(255,0,0)
#define Green RGB(0,110,0)
#define Blue RGB(0,0,255)
#define Yellow RGB(255,255,0)
#define Black RGB(0,0,0)



void AddMenus(HWND hwnd);
HMENU hMenu ;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_SIZEALL);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); //Change the background of window to be white

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
//////////////////////////////handling Shapes defination Part////////////////////////////
class Shape
{
public:
    int x1,y1,x2,y2,ID;
    COLORREF c;
    virtual ~Shape();
    virtual void draw(HDC hdc)=0;
};

class Line : public Shape
{
public:

    Line(int x1,int y1,int x2,int y2,COLORREF c,int ID);
    void draw(HDC hdc);
};

class Circle : public Shape
{
public:
    Circle(int x1,int y1,int x2,int y2,COLORREF c,int ID);
    void draw(HDC hdc);
};

class QuarterFilledCircle : public Shape
{
public:
    QuarterFilledCircle(int x1,int y1,int x2,int y2,COLORREF c,int ID);
    void draw(HDC hdc);
};

class Ellipsee : public Shape
{
public:
    int x3,y3;
    Ellipsee(int x1,int y1,int x2,int y2,int x3,int y3,COLORREF c,int ID);
    void draw(HDC hdc);
};

class Shapevector
{
public:
    vector<Shape*> shapes;
    void addshape(Shape* shape);
    void drawShapes(HDC hdc);
    void saveshapes(string path);
    void loadshapes(string path);
    void clearshapes();
};

//////////////////////////////line/////////////////////////////////////
void Draw_Parametric_Line(HDC hdc, int x1, int y1, int x2, int y2,COLORREF c)
{
    for(double t =0 ; t<1 ; t+=0.0001)
    {
        double x = x1 + t*(x2-x1);
        double y = y1 +t*(y2-y1) ;
        SetPixel(hdc, x, y,c) ;
    }
}

void Draw_Midpoint_Line(HDC hdc, int x1,int y1,int x2,int y2,COLORREF c)
{
    int  dx=x2-x1;
    int dy=y2-y1;
    if(abs(dy) <= abs(dx)) //dx is independent
    {
        if(dx <0)
        {
            swap(x1,x2);
            swap(y1,y2);
            dx = -dx;
            dy = -dy;
        }
        int x=x1, y=y1;
        SetPixel(hdc, x, y, c);
        if(dy>=0) // slope <1
        {
            int d = dx - 2 * dy;
            int d1 = 2*dx - 2*dy;
            int d2 = -2*dy;
            while(x<x2)
            {
                if(d<=0)
                {
                    d+=d1;
                    y++;
                }
                else
                {
                    d+=d2;
                }
                x++;
                SetPixel(hdc, x, y, c);
            }
        }
        else //-1<slope<0
        {
            int d = -dx - 2 * dy;
            int d1 = -2*dx - 2*dy;
            int d2 = 2*dy;

            while(x<x2)
            {
                if(d<=0)
                {
                    d+=d1;
                    y--;
                }
                else
                {
                    d+=d2;
                }
                x++;
                SetPixel(hdc, x, y, c);
            }
        }
    }
    else //dy is independent
    {
        if(dy<0)
        {
            swap(x1,x2);
            swap(y1,y2);

            dx = -dx;
            dy = -dy;
        }

        int x=x1, y=y1;
        SetPixel(hdc, x, y, c);
        if(dx>=0) // slope>1
        {
            double d = 2*dx - dy;
            int d1 = 2*dx - 2*dy;
            int d2 = 2*dx;

            while(y<y2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x++;
                }
                else
                {
                    d+=d2;
                }
                y++;
                SetPixel(hdc, x, y, c);
            }
        }
        else
        {
            double d = 2*dx + dy;
            int d1 = 2*dx + 2*dy;
            int d2 = 2*dx;

            while(y<y2)
            {
                if(d<=0)
                {
                    d+=d1;
                    x--;
                }
                else
                {
                    d+=d2;
                }
                y++;
                SetPixel(hdc, x, y, c);
            }
        }
    }
}


int Round(double x)
{
    return (int) x+0.5;
}

void Draw_DDA_Line(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx=x2-x1,dy=y2-y1;
    if(abs(dy)<=abs(dx))
    {
        if(x2<x1)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double m= (double)dy/dx;
        int x=x1;
        double y=y1;
        SetPixel(hdc,x,y,c);
        while(x<x2)
        {
            x++;
            y+=m;
            SetPixel(hdc,x,Round(y),c);
        }
    }
    else
    {
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        double m=(double)dx/dy;
        int y=y1;
        double x=x1;
        SetPixel(hdc,x,y,c);
        while(y<y2)
        {
            y++;
            x+=m;
            SetPixel(hdc,Round(x),y,c);
        }
    }
}
//////////////////////////////Circle part////////////////////////////////////
void Draw8Points(HDC hdc,int xc,int yc,int a, int b, COLORREF c)
{
    SetPixel(hdc, xc+a, yc+b, c);
    SetPixel(hdc, xc-a, yc+b, c);
    SetPixel(hdc, xc-a, yc-b, c);
    SetPixel(hdc, xc+a, yc-b, c);
    SetPixel(hdc, xc+b, yc+a, c);
    SetPixel(hdc, xc-b, yc+a, c);
    SetPixel(hdc, xc-b, yc-a, c);
    SetPixel(hdc, xc+b, yc-a, c);
}
void Draw_Direct_Circle(HDC hdc,int xc,int yc, int R, COLORREF c)
{
    //second eighter
    int x = 0 ;
    int y = R ;
    Draw8Points(hdc,xc,yc,0,R,c);
    while(x<y)
    {
        x++;
        y = sqrt(R*R - x*x) ;
        Draw8Points(hdc,xc,yc,x,Round(y),c);
    }
}

void Draw_Polar_Circle(HDC hdc, int xc,int yc, int R, COLORREF c)
{
    //first eighter
    int x = R ;
    int y = 0 ;
    Draw8Points(hdc,xc,yc,R,0,c);
    double theta = 0 ;
    double dTheta = 1.0/R ;
    while (x>y)
    {
        theta += dTheta ;
        x = R*cos(theta);
        y = R*sin(theta);
        Draw8Points(hdc,xc,yc,Round(x),Round(y),c);
    }
}
void Draw_Iterative_Polar_Circle (HDC hdc, int xc,int yc, int R, COLORREF c)
{
    //first eighter
    double dt=1.0/R;
    double cs = cos(dt);
    double ss =sin(dt);
    double x=R,y=0;
    Draw8Points(hdc,xc,yc,x,y,c);
    while(x>y)
    {
        double x1= x*cs-y*ss;
        y=x*ss+y*cs;
        x=x1;
        Draw8Points(hdc,xc,yc,Round(x),Round(y),c);
    }
}

void Draw_Circle_Midpoint(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0 ;
    int y = R ;
    Draw8Points(hdc,xc,yc,0,R,c);
    int d = 1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+= 2*x+3;
        }
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
        x++;
        Draw8Points(hdc,xc,yc,x,y,c);
    }
}
void Draw_Circle_Midpoint_Modification(HDC hdc, int xc, int yc, int R, COLORREF c)
{
    int x = 0 ;
    int y = R ;
    Draw8Points(hdc,xc,yc,x,y,c);
    int d = 1-R;
    int d1 = 3;
    int d2 = 5-2*R ;
    while (x<y)
    {
        if(d<0)
        {
            d+=d1;
            d1+=2;
            d2+=2;
        }
        else
        {
            d+=d2;
            d1+=2;
            d2+=4;
            y--;
        }
        x++;
        Draw8Points(hdc,xc,yc,x,y,c);
    }
}

/////////////////////////////Filling Part/////////////////////////////
void Filling_First_Quarter(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double x=0;
    double y=R;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        Draw_DDA_Line(hdc,xc,yc,xc+x,yc-y,c);
        Draw_DDA_Line(hdc,xc,yc,xc+y,yc-x,c);
    }
}
void Filling_Second_Quarter(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double x=0;
    double y=R;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        Draw_DDA_Line(hdc,xc,yc,xc-x,yc-y,c);
        Draw_DDA_Line(hdc,xc,yc,xc-y,yc-x,c);

    }
}
void Filling_Third_Quarter(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double x=0;
    double y=R;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        Draw_DDA_Line(hdc,xc,yc,xc-x,yc+y,c);
        Draw_DDA_Line(hdc,xc,yc,xc-y,yc+x,c);

    }
}
void Filling_Fourth_Quarter(HDC hdc,int xc,int yc,int R,COLORREF c)
{
    double x=0;
    double y=R;
    double d=1-R;
    while(x<y)
    {
        if(d<0)
        {
            d+=2*x+3;
        }
        else
        {
            d+=2*(x-y)-5;
            y--;
        }
        x++;
        Draw_DDA_Line(hdc,xc,yc,xc+x,yc+y,c);
        Draw_DDA_Line(hdc,xc,yc,xc+y,yc+x,c);

    }
}
void FillingQCircle(HDC hdc, int xc, int yc, int R, COLORREF c,int ID)
{
    if(ID==16)
        Filling_First_Quarter(hdc,xc,yc,R,c);
    else if(ID==17)
        Filling_Second_Quarter(hdc,xc,yc,R,c);
    else if (ID==18)
        Filling_Third_Quarter(hdc,xc,yc,R,c);
    else if(ID==19)
        Filling_Fourth_Quarter(hdc,xc,yc,R,c);

}


//////////////////////Elipse part///////////////////////////
void Draw4points(HDC hdc, int xc, int yc, int x, int y, COLORREF c)
{
    SetPixel(hdc,xc+x,yc+y,c);
    SetPixel(hdc,xc-x,yc+y,c);
    SetPixel(hdc,xc+x,yc-y,c);
    SetPixel(hdc,xc-x,yc-y,c);
}
void Draw_Elipse_Direct(HDC hdc, int xc, int yc, double A, double B, COLORREF c)
{
    double x = 0 ;
    double y = B ;
    Draw4points(hdc,xc,yc,x,y,c);
    while((x*(B*B)) < (y*(A*A)))
    {
        x++;
        y = sqrt(B*B *(1-((pow(x,2)/(pow(A,2))))));
        Draw4points(hdc,xc,yc,Round(x),Round(y),c);
    }
    x = A, y=0 ;
    Draw4points(hdc,xc,yc,x,y,c);
    while((y*(A*A)) < (x*(B*B)))
    {
        y++;
        x = sqrt(A*A *(1-((pow(y,2)/(pow(B,2))))));
        Draw4points(hdc,xc,yc,Round(x),Round(y),c);
    }

}

void Draw_Elipse_Polar(HDC hdc, int xc,int yc, double A, double B, COLORREF c)
{
    double x = A ;
    double y = 0 ;
    double dt = 1.0/max(A,B);
    for (double theta =0 ; theta <45 ; theta +=dt)
    {
        x = A * cos(theta);
        y = B * sin(theta);
        Draw4points(hdc,xc,yc,Round(x),Round(y),c);
    }
}

//////////////////////////////Clipping Part////////////////////////////
void PointClip(HDC hdc,int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF c)
{
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
    {
         SetPixel(hdc,x,y,c);
         cout<<"point is accepted.."<<endl;
    }
    else{
         cout<<"point is rejected..!!";
    }
}

union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
    *yi=y;
}
void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs;
    double y1=ys;
    double x2=xe;
    double y2=ye;
    OutCode out1 = GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2 = GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi;
        double yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {
        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}
//////////////////////////////handling Shapes declaration Part////////////////////////////

Line::Line(int x1,int y1,int x2,int y2,COLORREF c,int ID)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    this->c=c;
    this->ID=ID;
}
void Line::draw(HDC hdc)
{
    if(this->ID==8)
        Draw_Parametric_Line(hdc,this->x1,this->y1,this->x2,this->y2,this->c);
    else if(this->ID==9)
        Draw_Midpoint_Line(hdc,this->x1,this->y1,this->x2,this->y2,this->c);
    else if(this->ID==10)
        Draw_DDA_Line(hdc,this->x1,this->y1,this->x2,this->y2,this->c);
}


Circle::Circle(int x1,int y1,int x2,int y2,COLORREF c,int ID)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    this->c=c;
    this->ID=ID;
}
void Circle::draw(HDC hdc)
{
    int radius= sqrt(pow(this->x2 - this->x1,2)+pow(this->y2 - this->y1,2));
    if(this->ID==11)
        Draw_Direct_Circle(hdc,this->x1,this->y1,radius,this->c);
    else if(this->ID==12)
        Draw_Iterative_Polar_Circle(hdc,this->x1,this->y1,radius,this->c);
    else if(this->ID==13)
        Draw_Polar_Circle(hdc,this->x1,this->y1,radius,this->c);
    else if(this->ID==14)
        Draw_Circle_Midpoint(hdc,this->x1,this->y1,radius,this->c) ;
    else if(this->ID==15)
        Draw_Circle_Midpoint_Modification(hdc,this->x1,this->y1,radius,this->c);
}


QuarterFilledCircle::QuarterFilledCircle(int x1,int y1,int x2,int y2,COLORREF c,int ID)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    this->c=c;
    this->ID=ID;
}
void QuarterFilledCircle::draw(HDC hdc)
{
    int radius= sqrt(pow(this->x2 - this->x1,2)+pow(this->y2 - this->y1,2));
    FillingQCircle(hdc,this->x1,this->y1,radius,this->c,this->ID);
}

Ellipsee::Ellipsee(int x1,int y1,int x2,int y2,int x3,int y3,COLORREF c,int ID)
{
    this->x1=x1;
    this->y1=y1;
    this->x2=x2;
    this->y2=y2;
    this->x3=x3;
    this->y3=y3;
    this->c=c;
    this->ID=ID;
}
void Ellipsee::draw(HDC hdc)
{
    int A= sqrt(pow(this->x2 - this->x1,2)+pow(this->y2 - this->y1,2));
    int B= sqrt(pow(this->x3 - this->x1,2)+pow(this->y3 - this->y1,2));
    if(this->ID==20)
        Draw_Elipse_Direct(hdc,x1,y1,A,B,this->c);
    else if(this->ID==21)
        Draw_Elipse_Polar(hdc,x1,y1,A,B,this->c);
}

void Shapevector::addshape(Shape* shape)
{
    this->shapes.push_back(shape);
}
void Shapevector::drawShapes(HDC hdc)
{
    for(unsigned int i=0; i<shapes.size(); i++)
    {
        shapes[i]->draw(hdc);
    }
}
void Shapevector::saveshapes(string path)
{
    ofstream save(path);
    {
        for(unsigned int i =0 ; i<this->shapes.size(); i++)
        {
            save<<shapes[i]->ID<<"\t"<<shapes[i]->x1<<"\t"<<shapes[i]->y1<<"\t"<<shapes[i]->x2<<"\t"<<shapes[i]->y2<<"\t"<<shapes[i]->c<<endl;
        }
    }
}
void Shapevector::loadshapes(string path)
{
    this->clearshapes();
    ifstream load (path);
    while(!load.eof())
    {
        int ID,x1,y1,x2,y2;
        COLORREF color;
        load>>ID>>x1>>y1>>x2>>y2>>color;
        if(ID == 8 || ID == 9 || ID == 10)
        {
            Shape* nline=new Line(x1,y1,x2,y2,color,ID);
            this->addshape(nline);
        }
        else if(ID == 11 || ID == 12 || ID == 13 || ID == 14 || ID == 15)
        {
            Shape* ncircle=new Circle(x1,y1,x2,y2,color,ID);
            this->addshape(ncircle);

        }
        else if(ID==20||ID==21)
        {
            Shape* nellipsee=new Ellipsee(x1+(x2-x1)/2,y1+(y2-y1)/2,x1,y1+(y2-y1)/2,x1+(x2-x1)/2,y1,color,ID);
            this->addshape(nellipsee);
        }
        else if(ID == 16 || ID == 17 || ID==18 || ID==19 )
        {
            Shape* nfcircle=new QuarterFilledCircle(x1,y1,x2,y2,color,ID);
            this->addshape(nfcircle);
        }
    }
}
void Shapevector::clearshapes()
{
    for(unsigned int i=0; i<shapes.size(); i++)
    {
        delete shapes[i];
    }
    shapes.clear();
}
Shape::~Shape()
{
    //dtor
}

//////////////////////////////Menu Part////////////////////////////
void AddMenus(HWND hwnd)
{
    hMenu = CreateMenu();
    HMENU hFileMenu = CreateMenu();
    HMENU hColorMenue=CreateMenu();
    HMENU hLineMenu = CreateMenu();
    HMENU hCircleMenu = CreateMenu();
    HMENU hQuarterMenu = CreateMenu();
    HMENU hFillingMenu = CreateMenu();
    HMENU hElipse = CreateMenu();
    HMENU hClipping = CreateMenu();

    AppendMenu(hFileMenu,MF_STRING,FILE_MENU_SAVE,"Save");
    AppendMenu(hFileMenu,MF_STRING,FILE_MENU_LOAD,"Load");

    AppendMenu(hColorMenue,MF_STRING,Color_Menue_Red,"Red");
    AppendMenu(hColorMenue,MF_STRING,Color_Menue_Green,"Green");
    AppendMenu(hColorMenue,MF_STRING,Color_Menue_Blue,"Blue");
    AppendMenu(hColorMenue,MF_STRING,Color_Menue_Yellow,"Yellow");
    AppendMenu(hColorMenue,MF_STRING,Color_Menue_Black,"Black");

    AppendMenu(hLineMenu,MF_STRING,LINE_MENU_PARAMETRIC,"Parametric");
    AppendMenu(hLineMenu,MF_STRING,LINE_MENU_MIDPOINT,"Midpoint");
    AppendMenu(hLineMenu,MF_STRING,LINE_MENU_DDA,"DDA");

    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MENU_DIRECT,"Direct");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MENU_POLAR_ITERATIVE,"Polar Iterative");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MENU_POLAR,"Polar");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MENU_MIDPOINT,"Midpoint");
    AppendMenu(hCircleMenu,MF_STRING,CIRCLE_MENU_MODIFIED_MIDPOINT,"Modified-Midpoint");

    AppendMenu(hQuarterMenu,MF_STRING,FILLING_FIRST_QUARTER,"First Quarter");
    AppendMenu(hQuarterMenu,MF_STRING,FILLING_SECOND_QUARTER,"Second Quarter");
    AppendMenu(hQuarterMenu,MF_STRING,FILLING_THIRD_QUARTER,"Third Quarter");
    AppendMenu(hQuarterMenu,MF_STRING,FILLING_FOURTH_QUARTER,"Fourth Quarter");

    AppendMenu(hElipse,MF_STRING,ELIPSE_MENU_DIRECT,"Direct");
    AppendMenu(hElipse,MF_STRING,ELIPSE_MENU_POLAR,"Polar");

    AppendMenu(hClipping,MF_STRING,CLIPPING_MENU_POINT,"Point");
    AppendMenu(hClipping,MF_STRING,CLIPPING_MENU_LINE,"Line");

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"File");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hColorMenue,"Color");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hLineMenu,"Line");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hCircleMenu,"Circle");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hQuarterMenu,"Filling Circle");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hElipse,"Elipse");
    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hClipping,"Clipping");

    AppendMenu(hMenu,MF_STRING,Clear_Menu,"Clear");

    SetMenu(hwnd,hMenu);
}

string load(HWND hwnd)
{
    string path;
    OPENFILENAME ofn;
    char file_name[100];
    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=file_name;
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile=100;
    ofn.lpstrFilter="Text Files\0*.txt\0";
    ofn.nFilterIndex=1;

    GetOpenFileName(&ofn);
    path=ofn.lpstrFile;
    return path;
}
string save(HWND hwnd)
{
    string path;
    OPENFILENAME ofn;
    char file_name[100];
    ZeroMemory(&ofn,sizeof(OPENFILENAME));

    ofn.lStructSize=sizeof(OPENFILENAME);
    ofn.hwndOwner=hwnd;
    ofn.lpstrFile=file_name;
    ofn.lpstrFile[0]='\0';
    ofn.nMaxFile=100;
    ofn.lpstrFilter="Text Files\0*.txt\0";
    ofn.nFilterIndex=1;
    ofn.lpstrDefExt="*.txt";
    GetSaveFileName(&ofn);
    path=ofn.lpstrFile;
    return path;
}

COLORREF color=Red;
int x_1, y_1, x_2,y_2 ;
int x_c, y_c,x_r, y_r ;
int x_e, y_e, x_h, y_h;
int clipping_counter = 0 ;
int clipping_point_counter = 0;
int x_left,y_top,x_right,y_bottom,x_ss,y_ss,x_ee,y_ee;
int x_leftt,y_topp,x_rightt,y_bottomm,x_point,y_point;
int ID = 0;
Shapevector shapes;
Shape* nfcircle;
string path;


/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc=GetDC(hwnd);
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        AddMenus(hwnd);
        break;
    case WM_COMMAND:
        switch(wParam)
        {
        case Clear_Menu :
            shapes.clearshapes();
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        case  FILE_MENU_SAVE :
            path=save(hwnd);
            if(path!="")
                shapes.saveshapes(path);
            break;
        case  FILE_MENU_LOAD :
            path=load(hwnd);
            if(path!="")
                shapes.loadshapes(path);
            shapes.drawShapes(hdc);
            break;
        case Color_Menue_Red :
            cout << "Change Color to Red\n";
            color = Red;
            break;
        case Color_Menue_Green:
            cout << "Change Color to Green\n";
            color = Green;
            break;
        case Color_Menue_Blue:
            cout << "Change Color to Blue\n";
            color = Blue;
            break;
        case Color_Menue_Yellow:
            cout << "Change Color to Yellow\n";
            color = Yellow;
            break;
        case Color_Menue_Black:
            cout << "Change Color to Black\n";
            color = Black;
            break;
        case LINE_MENU_PARAMETRIC:
            cout << "Draw Parametric Line\n";
            ID =LINE_MENU_PARAMETRIC ;
            break;
        case LINE_MENU_MIDPOINT:
            cout << "Draw Midpoint Line\n";
            ID = LINE_MENU_MIDPOINT;
            break;
        case LINE_MENU_DDA:
            cout << "Draw DDA Line\n";
            ID = LINE_MENU_DDA;
            break;
        case CIRCLE_MENU_DIRECT:
            cout << "Draw Direct Circle\n";
            ID = CIRCLE_MENU_DIRECT;
            break;
        case CIRCLE_MENU_POLAR_ITERATIVE:
            cout << "Draw Polar Iterative Circle\n";
            ID = CIRCLE_MENU_POLAR_ITERATIVE;
            break;
        case CIRCLE_MENU_POLAR:
            cout << "Draw Polar Circle\n";
            ID = CIRCLE_MENU_POLAR;
            break;
        case CIRCLE_MENU_MIDPOINT:
            cout << "Draw Midpoint Circle\n";
            ID = CIRCLE_MENU_MIDPOINT;
            break;
        case CIRCLE_MENU_MODIFIED_MIDPOINT:
            cout << "Draw Modified Midpoint Circle\n";
            ID = CIRCLE_MENU_MODIFIED_MIDPOINT;
            break;
        case FILLING_FIRST_QUARTER:
            cout << "Filling First Quarter\n";
            nfcircle=new QuarterFilledCircle(x_c,y_c,x_r,y_r,color,FILLING_FIRST_QUARTER);
            nfcircle->draw(hdc);
            shapes.addshape(nfcircle);
            break;
        case FILLING_SECOND_QUARTER:
            cout << "Filling Second Quarter\n";
            nfcircle=new QuarterFilledCircle(x_c,y_c,x_r,y_r,color,FILLING_SECOND_QUARTER);
            nfcircle->draw(hdc);
            shapes.addshape(nfcircle);
            break;
        case FILLING_THIRD_QUARTER:
            cout << "Filling Third Quarter\n";
            nfcircle=new QuarterFilledCircle(x_c,y_c,x_r,y_r,color,FILLING_THIRD_QUARTER);
            nfcircle->draw(hdc);
            shapes.addshape(nfcircle);
            break;
        case FILLING_FOURTH_QUARTER:
            cout << "Filling Fourth Quarter\n";
            nfcircle=new QuarterFilledCircle(x_c,y_c,x_r,y_r,color,FILLING_FOURTH_QUARTER);
            nfcircle->draw(hdc);
            shapes.addshape(nfcircle);
            break;
        case ELIPSE_MENU_DIRECT:
            cout << "Draw Direct Elipse\n";
            ID = ELIPSE_MENU_DIRECT;
            break;
        case ELIPSE_MENU_POLAR:
            cout << "Draw Polar Elipse\n";
            ID = ELIPSE_MENU_POLAR;
            break;
        case CLIPPING_MENU_POINT:
            cout << "Clipping Point\n";
            ID = CLIPPING_MENU_POINT;
            break;
        case CLIPPING_MENU_LINE:
            cout << "Clipping line\n";
            ID = CLIPPING_MENU_LINE;
            break;
        }
        break;
    case WM_LBUTTONDBLCLK:
        if (ID == 8 || ID == 9 || ID == 10  )
        {
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            auto x  = GET_X_LPARAM(lParam);
            auto y  = GET_Y_LPARAM(lParam);
            printf("First click: %d %d\n",x,y);
        }
        if (ID == 11 || ID == 12 || ID == 13 || ID == 14 || ID == 15)
        {
            x_c = LOWORD(lParam);
            y_c = HIWORD(lParam);
            auto x  = GET_X_LPARAM(lParam);
            auto y  = GET_Y_LPARAM(lParam);
            printf("First click: %d %d\n",x,y);
        }
        if (ID == 20 || ID == 21 )
        {
            x_e = LOWORD(lParam);
            y_e = HIWORD(lParam);
            auto x  = GET_X_LPARAM(lParam);
            auto y  = GET_Y_LPARAM(lParam);
            printf("First click: %d %d\n",x,y);
        }
        if(ID == 22)
        {
            if(clipping_point_counter == 0)
            {
                x_leftt=LOWORD(lParam);
                y_topp=HIWORD(lParam);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("X_Left click: %d %d\n",x);
                printf("Y_Top click: %d %d\n",y);
                clipping_point_counter++;
            }
            else if (clipping_point_counter==1)
            {
                x_rightt=LOWORD(lParam);
                y_bottomm=HIWORD(lParam);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("X_Right click: %d %d\n",x);
                printf("Y_Top click: %d %d\n",y);
                Rectangle(hdc, x_leftt,y_topp,x_rightt,y_bottomm);
                clipping_point_counter++;
            }
            else if (clipping_point_counter == 2)
            {
                x_point = LOWORD(lParam);
                y_point = HIWORD(lParam);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("X-Coordinate of the point: %d %d\n",x);
                printf("Y-Coordinate of the point: %d %d\n",y);
                PointClip(hdc,x_point,y_point,x_leftt,y_topp,x_rightt,y_bottomm,color);
                clipping_point_counter=2;

            }
        }
        if (ID == 23)
        {
            if (clipping_counter == 0)
            {
                x_left=LOWORD(lParam);
                y_top=HIWORD(lParam);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("X_Left click: %d %d\n",x);
                printf("Y_Top click: %d %d\n",y);
                clipping_counter ++ ;
            }
            else if (clipping_counter == 1)
            {
                x_right=LOWORD(lParam);
                y_bottom=HIWORD(lParam);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("X_Right click: %d %d\n",x);
                printf("Y_Top click: %d %d\n",y);
                Rectangle(hdc, x_left,y_top,x_right,y_bottom);
                clipping_counter ++ ;
            }
            else if (clipping_counter == 2)
            {
                x_ss = LOWORD(lParam);
                y_ss = HIWORD(lParam);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("First click: %d %d\n",x,y);
                clipping_counter ++ ;
            }
            else if (clipping_counter == 3)
            {
                x_ee = LOWORD(lParam);
                y_ee = HIWORD(lParam);
                CohenSuth(hdc,x_ss,y_ss,x_ee,y_ee,x_left,y_top,x_right,y_bottom);
                auto x  = GET_X_LPARAM(lParam);
                auto y  = GET_Y_LPARAM(lParam);
                printf("Second click: %d %d\n",x,y);
                clipping_counter = 2 ;
            }
        }
        break;
    case WM_RBUTTONDBLCLK:
        if (ID == 8 || ID == 9 || ID == 10)
        {
            x_2 = LOWORD(lParam);
            y_2 = HIWORD(lParam);
            auto x  = GET_X_LPARAM(lParam);
            auto y  = GET_Y_LPARAM(lParam);
            printf("Second click: %d %d\n",x,y);
            Shape* nline=new Line(x_1,y_1,x_2,y_2,color,ID);
            nline->draw(hdc);
            shapes.addshape(nline);
        }
        if (ID == 11 || ID == 12 || ID == 13 || ID == 14 || ID == 15)
        {
            x_r = LOWORD(lParam);
            y_r = HIWORD(lParam);
            auto x  = GET_X_LPARAM(lParam);
            auto y  = GET_Y_LPARAM(lParam);
            printf("Second click: %d %d\n",x,y);
            Shape* ncircle=new Circle(x_c,y_c,x_r,y_r,color,ID);
            ncircle->draw(hdc);
            shapes.addshape(ncircle);
        }

        if (ID == 20 || ID == 21)
        {
            x_h = LOWORD(lParam);
            y_h = HIWORD(lParam);
            auto x  = GET_X_LPARAM(lParam);
            auto y  = GET_Y_LPARAM(lParam);
            printf("Second click: %d %d\n",x,y);
            Shape* nellipsee=new Ellipsee(x_e+(x_h-x_e)/2,y_e+(y_h-y_e)/2,x_e,y_e+(y_h-y_e)/2,x_e+(x_h-x_e)/2,y_e,color,ID);
            nellipsee->draw(hdc);
            shapes.addshape(nellipsee);
        }
        break;
    case WM_SIZE:
        switch(wParam)
        {
        case SIZE_RESTORED:
            shapes.drawShapes(hdc);
            break;
        }
        break;
    case WM_DESTROY:
        shapes.clearshapes();
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
