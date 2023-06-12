#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <algorithm>
#include <vector>
#include <bits/stdc++.h>
#include<stdlib.h>
//using std::vector;
//using std::swap;
using namespace std;

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
    wincl.hCursor = LoadCursor (NULL, IDC_HAND); //change cursor
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) (CreateSolidBrush(RGB(255,255,255))); //change color of window to be white

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
bool random=false;

struct point
{
    int x,y;
    point(int x,int y):x(x),y(y) {}
    point() {}
};
vector <point> v;



void Pixel(HDC hdc,int x, int y, COLORREF c)
{
    SetPixel(hdc, x,y,c);
    point m;
    m.x=x;
    m.y=y;
    v.push_back(m);
}


/////////////////////////CIRCLE///////////////////
void Draw8Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color) //utility function
{
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc+b, yc+a, color);
    SetPixel(hdc, xc-b, yc+a, color);
    SetPixel(hdc, xc-b, yc-a, color);
    SetPixel(hdc, xc+b, yc-a, color);
}
void CircleDirect(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int R_sqr=R*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        x++;
        y=round(sqrt((double)(R_sqr-x*x)));
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}

void Circlemid(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
            d+=2*x+2;
        else
        {
            d+=2*(x-y)+5;
            y--;
        }
    x++;
    Draw8Points(hdc,xc,yc,x,y,color);
    }
}

void Circle_IterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double c=cos(dtheta),s=sin(dtheta);
    Draw8Points(hdc,xc,yc,R,0,color);
    while(x+20>y)
    {
        double x1=(x*c)-(y*s);
        y=(x*s)+(y*c);
        x=x1;
        Draw8Points(hdc,xc,yc,round(x),round(y),color);
    }
}

void Circlemodified_mid(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    int x=0,y=R;
    int d=1-R;
        int c1=3, c2=5-2*R;
    Draw8Points(hdc,xc,yc,x,y,color);
    while(x<y)
    {
        if(d<0)
        {
            d+=c1;
            c2+=2;
        }
        else
        {
            d+=c2;
            c2+=4;
            y--;
        }
        c1+=2;
        x++;
        Draw8Points(hdc,xc,yc,x,y,color);
    }
}




//////////////////////LINE///////////////////////////
////line parametric/////
void Parametricalgorithm (HDC hdc, int x1, int y1,int x2, int y2, COLORREF c)
{
    double dt = 1.0/(max(abs(x2-x1), abs(y2-y1)));
    for(double t =0 ; t<=1 ; t+=dt)
    {
        double x = x1+t*(x2-x1);
        double y = y1+t * (y2-y1);
        Pixel( hdc, x, y, c);
    }
}

////line DDA/////
int ROUND(double x) //utility function
{
    return (int)(x+0.5);
}

void DrawLine_DDA(HDC hdc, int x1,int y1,int x2,int y2,COLORREF color)
{
    int dx=x2-x1;
    int dy=y2-y1;
    if(abs(dx)>=abs(dy))
    {
        double m=(double)dy/dx;
        if(x1>x2)
        {
            swap(x1,x2);
            swap(y1,y2);
        }
        int x=x1;
        double y=y1;
        SetPixel(hdc,x1,y1,color);
        while(x<x2)
        {
            x++;
            y+=m;
            SetPixel(hdc,x,ROUND(y),color);}}
    else
    {
        double m=(double)dx/dy;
        if(y1>y2)
        {
            swap(x1,x2);
            swap(y1,y2);}
        double x=x1;
        int y=y1;
        SetPixel(hdc,x1,y1,color);
        while(y<y2)
        {
            y++;
            x+=m;
            SetPixel(hdc,ROUND(x),y,color);
        }

    }
}

/////line bresenham////////
void Bresenhamalgoritms (HDC hdc, int x1, int y1,int x2, int y2, COLORREF c)
{
    int dx=x2-x1;
    int  dy=y2-y1;
    if(abs(dx)>abs(dy))
    {
        if(dx<=0)
        {
            swap(x1,x2);
            swap(y1,y2);
            dx=-dx;
            dy=-dy;
        }
        if(dy>=0)
        {
            int x=x1;
            int y=y1;
            while(x<x2)
            {
                int d=(2*y+1-(2*y1))*dx-(2*(x+1-x1)*dy);
                if(d<=0)
                {
                    x++;
                    y++;
                    Pixel(hdc,x,y,c);
                }
                else
                {
                    x++;
                    Pixel(hdc,x,y,c);
                }
            }

        }
        else
        {
            int x=x1;
            int y=y1;
            while(x<x2)
            {
                int d=((((2*y)+1)-(2*y1))*dx)-(2*(x+1-x1)*abs(dy));
                if(d<=0)
                {
                    x++;
                    y--;
                    Pixel(hdc,x,y,c);
                }
                else
                {

                    x++;
                    Pixel(hdc,x,y,c);
                }
            }


        }
    }//dy>dx
    else
    {
        if(dy<0)
        {
            swap(x1,x2);
            swap(y1,y2);
            dx=-dx;
            dy=-dy;
        }
        if(dx>=0)
        {
            int x=x1;
            int y=y1;
            while(y<y2)
            {
                int d=2*(((y+1)-y1)*dx)-((((2*x)+1)-(2*x1))*dy);
                if(d>=0)
                {
                    x++;
                    y++;
                    Pixel(hdc,x,y,c);

                }
                else
                {
                    y++;
                    Pixel(hdc,x,y,c);
                }
            }

        }
        else// dx<0
        {
            int x=x1;
            int y=y1;
            while(y<y2)
            {
                int d=2*(y+1-y1)*abs(dx)-(((2*x+1)-(2*x1))*dy);
                if(d>=0)
                {
                    y++;
                    x--;
                    Pixel(hdc,x,y,c);
                }

                else
                {
                    y++;
                    Pixel(hdc,x,y,c);

                }
            }

        }
    }

}

////////////////////////////////FILLING/////////////////////////////
void Filling(HDC hdc, int xc,int yc,int a,int b,int R,COLORREF color,int q){



    int x=R,y=0;
    double theta=0,dtheta=0.09;
        if(q==4){
        DrawLine_DDA(hdc,xc,yc,xc-x,yc-y,color);
        }
        else if(q==2){

         DrawLine_DDA(hdc,xc,yc,xc+x,yc+y,color);

        }
        else if(q==1){

        DrawLine_DDA(hdc,xc,yc,xc+x,yc-y,color);

        }
        else if(q==3){
        DrawLine_DDA(hdc,xc,yc,xc-x,yc+y,color);
        }
    while(x*10>y)
    {
        theta+=dtheta;
        x=round(R*cos(theta));
        y=round(R*sin(theta));
        if(q==4){
        DrawLine_DDA(hdc,xc,yc,xc-x,yc-y,color);
        }
        else if(q==2){

         DrawLine_DDA(hdc,xc,yc,xc+x,yc+y,color);

        }
        else if(q==1){

        DrawLine_DDA(hdc,xc,yc,xc+x,yc-y,color);

        }
        else if(q==3){

        DrawLine_DDA(hdc,xc,yc,xc-x,yc+y,color);

        }
    }



}


/*void Draw_8_point_Filling(HDC hdc,int xc,int yc,int x,int y,int quarter,COLORREF color)
{
    if(quarter==2)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc-x,yc-y,color);
        DrawLine_DDA(hdc,xc,yc,xc-y,yc-x,color);}
    if(quarter==4)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc+x,yc+y,color);
        DrawLine_DDA(hdc,xc,yc,xc+y,yc+x,color);}
    if(quarter==3)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc-x,yc+y,color);
        DrawLine_DDA(hdc,xc,yc,xc-y,yc+x,color);}
    if(quarter==1)
    {
        SetPixel(hdc,xc+x,yc+y,color);
        SetPixel(hdc,xc-x,yc+y,color);
        SetPixel(hdc,xc+x,yc-y,color);
        SetPixel(hdc,xc-x,yc-y,color);
        SetPixel(hdc,xc+y,yc+x,color);
        SetPixel(hdc,xc-y,yc+x,color);
        SetPixel(hdc,xc+y,yc-x,color);
        SetPixel(hdc,xc-y,yc-x,color);
        DrawLine_DDA(hdc,xc,yc,xc+x,yc-y,color);
        DrawLine_DDA(hdc,xc,yc,xc+y,yc-x,color);}}
void DrawCircle_Filling(HDC hdc,int xc,int yc,int R,int quarter,COLORREF color)
{
    double dtheta=1.0/R;
    double c=cos(dtheta);
    double s=sin(dtheta);
    double x=R;
    double y=0;
    Draw_8_point_Filling(hdc,xc,yc,R,0,quarter,color);
    while (x>y)
    {
        double x1=x*c-y*s;
        y=x*s+y*c;
        x=x1;
        Draw_8_point_Filling(hdc,xc,yc,ROUND(x),ROUND(y),quarter,color);}}*/
////////////////////////////////Ellipse/////////////////////////////
void Draw4Points (HDC hdc, int x, int y, int xc, int yc, COLORREF c ) //utility function
{
    Pixel(hdc, xc+x, yc+y, c);
    Pixel(hdc, xc-x, yc+y, c);
    Pixel(hdc, xc+x, yc-y, c);
    Pixel(hdc, xc-x, yc-y, c);
}
///////////////////////////////////// Direct algorithms //////////////////////////////////////
/*void  DirectEllipse (HDC hdc,double rx, int ry,int xc, int yc, COLORREF c)
{
    double x =rx;
    int y = 0 ;
    Draw4Points ( hdc,  x,  y,  xc,  yc, c );

    while(x>=0)
    {
        x-=0.1;
        y = abs(ry*sqrt(1-(pow(x,2.0)/pow(rx,2.0)))) ;
        Draw4Points ( hdc,  x,  y,  xc,  yc, c );
    }
}*/
void EllipseDirect(HDC hdc, int xc,int yc,int a,int b,COLORREF color){

    int x = 0, y = b;
        Draw4Points(hdc,xc,yc,x,y,color);
    while (x*(b*b) < ((a*a)*y))
    {
        x++;
        y =round(sqrt(((b*b)*((a*a)-(x*x)))/(a*a)));
        Draw4Points(hdc,xc,yc,x,y,color);
    }
    y = 0, x = a;
        Draw4Points(hdc,xc,yc,x,y,color);
    while (x*(b*b) >((a*a)*y))
    {
        y++;
        x = round(sqrt(((a*a)*((b*b) - (y * y)))/(b*b)));
        Draw4Points(hdc,xc,yc,x,y,color);
    }
}

////////////////////////////////// Direct  polar /////////////////////////////////
void EllipsePolar(HDC hdc,int xc,int yc, int R1,int R2,COLORREF color)
{

    int x=R1,y=0;
    double theta=0,dtheta=1.0/R1;
    Draw4Points(hdc,xc,yc,x,y,color);
    while(x+R1>y)
    {
        theta+=dtheta;
        x=round(R1*cos(theta));
        y=round(R2*sin(theta));
        Draw4Points(hdc,xc,yc,x,y,color);
    }

}


///////////////////LINE CLIPPING//////////////////////
union OutCode
{
    unsigned All:4;
    struct{unsigned left:1,top:1,right:1,bottom:1;};
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright, int ybottom) //utility function gets the side code of a point with respect to a window
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    else if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}

//the intersection utilities
VIntersect(double xs,double ys,double xe,double ye,int x, double *xi,double*yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}

HIntersect(double xs,double ys,double xe,double ye,int y,double*xi,double*yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        } else
            {
                if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
                else if(out2.top)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
                else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
                else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
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

///////////////////////////////POINT CLIPPING///////////////////////
void PointClipping(int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
    HWND hwnd;
    HDC hdc=GetDC(hwnd);
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
    Pixel(hdc,x,y,color);
}

/////////////////////////SAVE FUNCTION//////////////////////////////
void save(string name)
{
    ofstream file;
    file.open(name,ofstream::out | ofstream::trunc);
    for(int i = 0 ; i <v.size() ; i++)
    {
        file<<v[i].x<<'\n'<<v[i].y<<'\n';
    }
    file.close();
}
///////////////////////////LOAD FUNCTION/////////////////////////
void load(string name,HDC hdc,COLORREF c)
{
    ifstream file;
    file.open(name);
    int i = 0;
    vector<string> vect;
    while(!file.eof())
    {
        string x;
        file>>x;
        vect.push_back(x);
        i++;
    }
    point p;
    v.clear();

    for(int i = 0 ; i < vect.size()-1 ; i++)
    {
        int m;
        istringstream(vect[i])>>m;
        p.x = m;
        i++;
        istringstream(vect[i])>>m;
        p.y=m;
        v.push_back(p);
    }
    vect.clear();
    for(int i = 0 ; i <v.size() ; i++)
    {
        SetPixel(hdc,v[i].x,v[i].y,c);
    }
    file.close();

}


/*  This function is called by the Windows function DispatchMessage()  */
int Rc,Gc,Bc;
COLORREF c = 0;
//COLORREF cF = 0;
int x,y,x2,y2;
int xc,yc;
int xe,ye;
    int xe2,ye2;
    int R,R2;
    int windowX,windowY,windowR;
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static int shape;
    static int color=RGB(0,0,0);
    HDC hdc=GetDC(hwnd);
    if(random) //if the user chose a random color
    {
        Rc = (rand()* 10000) % 255;
        Gc = (rand()* 10000) % 180;
        Bc = (rand()* 10000) % 255;
        c = RGB(Rc,Gc,Bc);
    }

    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
        {
            cout<<wParam<<endl;
            if (wParam==22){
                color=RGB(0,0,0);

            }
            else if(wParam==23){

                color=RGB(255,0,0);


            }
            else if(wParam==24){

                color=RGB(0,0,255);


            }
            else if(wParam==25){

             color=RGB(0,255,0);

            }
            else if(wParam==26){
                random=true;

            }
            else if(wParam==27){
                save("Project.txt");
            }
            else if(wParam==28)
            {
                load("Project.txt",hdc,RGB(255,0,0));
            }
            else if(wParam==29)
            {
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if(wParam==12){

                Filling(hdc,xc,yc,xe,ye,R,color,1);

            }
            else if(wParam==13){

                Filling(hdc,xc,yc,xe,ye,R,color,2);
               }
            else if(wParam==14){

                Filling(hdc,xc,yc,xe,ye,R,color,3);
            }
            else if(wParam==15){

                Filling(hdc,xc,yc,xe,ye,R,color,4);
            }
            else{
                shape=wParam;
            }
            break;
        }

        case WM_LBUTTONDOWN:
            {
                xe=LOWORD(lParam);
                ye=HIWORD(lParam);
                ///////line////
                if(shape==1){

                Parametricalgorithm(hdc,xc,yc,xe,ye,color);

            }
                else if(shape==2){
                    DrawLine_DDA(hdc,xc,yc,xe,ye,color);
                }
                else if(shape==3){

                Bresenhamalgoritms(hdc,xc,yc,xe,ye,color);
            }
            //////////circle//////////////
             else if(shape==18){
                R = sqrt(pow((xe-xc), 2) + pow((ye-yc), 2));
                CircleDirect(hdc,xc,yc,R,color);

            }
               else if(shape==19){
                R = sqrt(pow((xe-xc), 2) + pow((ye-yc), 2));
                Circlemid(hdc,xc,yc,R,color);

            }
            else if(shape==20){
                R = sqrt(pow((xe-xc), 2) + pow((ye-yc), 2));
                Circle_IterativePolar(hdc,xc,yc,R,color);

            }
            else if(shape==21){
                R = sqrt(pow((xe-xc), 2) + pow((ye-yc), 2));
                Circlemodified_mid(hdc,xc,yc,R,color);

            }
            /////////Ellipse//////////
                else if(shape==4){

                EllipsePolar(hdc,xc,yc,xe,ye,color);
            }

            else if(shape==5){

                EllipseDirect(hdc,xc,yc,xe,ye,color);
            }
            /////Clipping////////

            else if(shape==16){

                CohenSuth(hdc,xc,yc,xe,ye,windowX,windowY,windowR,color);
            }
            break;
            }
        case WM_RBUTTONUP:
            {
                if(shape==4){

                xe2=LOWORD(lParam);
                ye2=HIWORD(lParam);
                //first radius
                R = sqrt(pow((xe-xc), 2) + pow((ye-yc), 2));

                //second radius
                R2=(int)sqrt(pow(xe2-xc,2.0)+pow(ye2-yc,2.0));
                //draw Ellipse with Polar algo
                EllipsePolar(hdc,xc,yc,R,R2,color);

            }
            break;
            }
      case WM_CREATE:
    {
        HICON hIcon;
        HMENU hmenu=CreateMenu();
        HMENU LINE=CreateMenu();
        //HMENU hFilling=CreateMenu();
        HMENU ELLIPSE=CreateMenu();
        HMENU FILE=CreateMenu();
        HMENU hquarter=CreateMenu();
        HMENU CLIPPING=CreateMenu();
        HMENU CIRCLE=CreateMenu();
        HMENU hColorMenu = CreateMenu();
        /////////////////////////////////////////////////////////////////////////////////
        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)LINE,"Line");
        AppendMenu(LINE,MF_STRING,1,"Parametric");
        AppendMenu(LINE,MF_STRING,2," DDA");
        AppendMenu(LINE,MF_STRING,3,"Bresenham's");
        /////////////////////////////////////////////////////////////////////////////////
        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)ELLIPSE,"Ellipse");
        AppendMenu(ELLIPSE,MF_STRING,4,"Polar");
        AppendMenu(ELLIPSE,MF_STRING,5,"Direct");
    /////////////////////////////////////////////////////////////
        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hquarter,"Filling");
        AppendMenu(hquarter,MF_STRING,12,"First");
        AppendMenu(hquarter,MF_STRING,13,"Second");
        AppendMenu(hquarter,MF_STRING,14,"Third");
        AppendMenu(hquarter,MF_STRING,15,"Fourth");
//////////////////////////////////////////////////////////////////////////////////////
        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)CLIPPING,"Clipping");
        AppendMenu(CLIPPING,MF_STRING,16,"Line Clipping");
        AppendMenu(CLIPPING,MF_STRING,17," Point Clipping");
       ///////////////////////////////////////////////////////////
       AppendMenu(hmenu,MF_POPUP,(UINT_PTR)CIRCLE,"Circle");
       AppendMenu(CIRCLE,MF_STRING,18,"Direct");
       AppendMenu(CIRCLE,MF_STRING,19,"Midpoint");
       AppendMenu(CIRCLE,MF_STRING,20,"Iterative Polar");
       AppendMenu(CIRCLE,MF_STRING,21,"Modified Bresenham");
       /////////////////////////////////////////////////////
       AppendMenu(hColorMenu,MF_STRING,22,"Black");
       AppendMenu(hColorMenu,MF_STRING,23,"Red");
       AppendMenu(hColorMenu,MF_STRING,24,"Blue");
       AppendMenu(hColorMenu,MF_STRING,25,"Green");
       AppendMenu(hColorMenu,MF_STRING,26,"Random");
       AppendMenu(hmenu,MF_POPUP,(UINT_PTR)hColorMenu,"Colors");
///////////////////////////////////////////////////////////////////////////////////
        AppendMenu(hmenu,MF_POPUP,(UINT_PTR)FILE,"File ");
        AppendMenu (FILE,MF_STRING,27, "Save");
        AppendMenu (FILE,MF_STRING,28, "Load");
        AppendMenu(FILE,MF_STRING,29,"Clear");
        SetMenu(hwnd,hmenu);
        break;
    }
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}




