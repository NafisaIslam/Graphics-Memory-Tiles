#include <iostream>
#include<stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include<GL/gl.h>
#include <GL/glut.h>
#include<math.h>
#include <windows.h>
#include<vector>
#include <string>
#include<cstring>
#include<bits/stdc++.h>  // it includes all c++ headers
using namespace std;
int sc = 0;
void update(int);
int windowHeight ;

double theGlutMouseXCoordinate ;
double theGlutMouseYCoordinate ;

int theGlutWindowWidth = 512 ;
int theGlutWindowHeight = 512 ;
double mouseX ;
double mouseY ;
int t;

class point
{
public:
    double x ;
    double y ;

    point( double _x = 0 , double _y = 0 )
    {
        x = _x ;
        y = _y ;
    }
};

class Square
{
public:

    point p1 ;
    point p2 ;
    point p3 ;
    point p4 ;

    int SID ;
    Square( point _p1 , point _p2 , point _p3 , point _p4 , int sid )
    {
        p1 = _p1 ;
        p2 = _p2 ;
        p3 = _p3 ;
        p4 = _p4 ;

        SID = sid ;
    }
    Square()
    {

    }

    bool isPointInside( point p )
    {

        bool inside = false ;
        if( p.x >= p1.x && p.x <= p2.x && p.y >= p1.y && p.y <= p4.y ) inside = true ;
        return inside ;
    }
};

class Color
{
public:
    int r , g , b ;
    int CID = 0 ;
    Color( int _r = 0, int _g = 0 , int _b = 0 , int _cid = 0 )
    {
        r = _r ;
        g = _g ;
        b = _b ;

        CID = _cid ;
    }
    void print()
    {
        cout << "[" << r << "][" << g << "][" << b <<"] = " << CID << endl ;
    }
};
//LIBRARY ENDS


//calibration

int BASE = 100 ; // it is length of each square ,
int BORDER = 10 ; // it is the gap between two squares


//VARS STARTS ;
vector<Square> vector_squares ;
vector<int> myvec ; // contains squared ids in random order from 0 to 15 // total 16 entry
vector<Color> vector_colors ; // contains 8 entries from 0 to 7 //  this are color ids
int hidden_colors[16] ; // hidden_colors[square id] = square_hidden_color_id
int isClicked[16] ; //if clicked value = 1 ;
int isSolved[16] ; //
int totalClick = 0 ; // maximum == 2
//VARS END ;






// METHODS
void renderBitmapString(float x,float y,char* string)
{
    ///p=0.0;
    int len;
    glColor4f(1.0,1.0,1.0,1.0);
    glRasterPos2i(x,y);
    glDisable(GL_TEXTURE);
    glDisable(GL_TEXTURE_2D);
    len=strlen(string);
    for (int i=0;i<len;i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15,(int)string[i]);
    }
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE);

}
bool isColorRED = false ;

void create16Squares()
{
    for( int sid = 0 ; sid < 16 ; sid++ )
    {
        int row = 0 ;
        int col = 0 ;

        row = sid/4 ;
        col = sid%4 ;

        int basicX = (col)*(BASE)+(col+1)*(BORDER) ;
        int basicY = (row)*(BASE)+(row+1)*(BORDER) ;

        Square square ;
        square.p1 = point(basicX,basicY) ;
        square.p2 = point(basicX+BASE,basicY) ;
        square.p3 = point(basicX+BASE,basicY+BASE) ;
        square.p4 = point(basicX,basicY+BASE) ;
        square.SID = sid ;
        vector_squares.push_back(square) ;

        }
}
int giveSquareID( int x , int y )
{
    int sid = -1 ;
    point p(x,y) ;
    for( int i = 0 ; i < 16 ; i++ )
    {
        if( vector_squares[i].isPointInside(p) )
        {
            sid = i ;
            break ;
        }
    }
    return sid ;
}
void mouseMotion(int x, int y)
{
    y = theGlutWindowHeight-y ;

    int sid = giveSquareID(x,y) ;

}


void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )   // Pause/resume
    {
        y = theGlutWindowHeight - y ;


        int sid = giveSquareID(x,y) ;
        if( sid >=0 && sid < 16)
        {
            totalClick++ ;
            cout << "cliked on : " << sid << endl ;

            isClicked[sid]++ ; // =  !isClicked[sid] ;

            if( totalClick==3)
            {
                isClicked[sid]-- ;
                totalClick = 0 ;

                glutPostRedisplay() ;

                int sid_1 = -1 ;
                int sid_2 = -1 ;
                for( int i = 0 ; i < 16 ; i++ )
                {
                    if( isSolved[i] == 0 && isClicked[i] >= 1 )
                    {
                        if( sid_1 == -1 )
                        {
                            sid_1 = i ;
                        }
                        else if( sid_1 >=  0 )
                        {
                            sid_2 = i ;
                        }
                    }
                }
                cout << "sid_1 = " << sid_1 << " sid_2 = " << sid_2 << endl ;
                if( sid_1 >= 0 && sid_2 >= 0 )
                {
                    //test color code
                    if( hidden_colors[sid_1] == hidden_colors[sid_2] )
                    {
                        cout << "MATCH !!!!!!!!!! " << endl ;
                        sc++;
                        isSolved[sid_1] = isSolved[sid_2] = 1 ;
                    }
                }



                for( int i = 0 ; i < 16 ; i++ )
                {

                    isClicked[i] = 0 ;
                }

            }
            else if( totalClick == 2 )
            {

                glutPostRedisplay() ;
            }
        }
    }
}

void drawSingleSquare( Square sq )
{

    int sid = sq.SID ;
    int colorID = hidden_colors[sq.SID] ;
    float rr = vector_colors[colorID].r ;
    float gg = vector_colors[colorID].g ;
    float bb = vector_colors[colorID].b ;

    rr /= 255 ;
    gg /= 255 ;
    bb /= 255 ;
    if( isClicked[sid] == 0 )
    {
        glColor3f(1,0,0) ; // default ;
    }
    else if( isClicked[sid] == 1 )
    {

        glColor3f(rr,gg,bb) ;
    }

    if( isSolved[sid] == 1 )
    {
        glColor3f(0,1,0) ;
    }
    glPushMatrix();
    {
        //glTranslatef(t,t,0) ;
        //t += 10 ;
        glBegin(GL_QUADS);
        {
            glVertex3f(sq.p1.x,sq.p1.y,0) ;
            glVertex3f(sq.p2.x,sq.p2.y,0) ;
            glVertex3f(sq.p3.x,sq.p3.y,0) ;
            glVertex3f(sq.p4.x,sq.p4.y,0) ;
        }
        glEnd();
    }
    glPopMatrix() ;



}
void draw16Squares()
{
    for( int sid = 0 ; sid < 16 ; sid++ )
    {
        drawSingleSquare( vector_squares[sid] ) ;
    }
char  str[100];
itoa(sc,str,10);

renderBitmapString(400,500,str);
}

void createRandomColorsAndAssignIntoSquares()
{
    // total 8 different colors
    for( int i = 0 ; i < 8 ; i++ )
    {

        int R = rand()%256+0.1 ;
        int G = rand()%256 ;
        int B = rand()%256 ;


        Color color(R,G,B,i) ;
        vector_colors.push_back(color) ;
    }
    for( int i = 0 ; i < 8 ; i++ )
    {
        //vector_colors[i].print() ;
    }

    //pairing order
    for( int i = 0 ; i < 16 ; i++ )
    {
        myvec.push_back(i) ;
    }
    random_shuffle(myvec.begin(),myvec.end()) ;
    /*
    for( int i = 0 ; i < 16 ; i++ )
    {
        cout << myvec[i] << " " ;
    }
    cout << endl ;
    */
    // assign in pairs ,
    int colorID = 0 ;

    for( int i = 0 ; i < 16 ;  )
    {
        int sid ;
        sid = myvec[i] ;
        hidden_colors[sid] = colorID ;
        i++ ;
        sid = myvec[i] ;
        hidden_colors[sid] = colorID ;
        i++ ;
        colorID++ ;
    }


}
bool creation_complete = false ;




void memoryTileGame()
{
    create16Squares() ;
    createRandomColorsAndAssignIntoSquares() ;

    creation_complete = true ;

}
//MYCODE ENDS

void myDisplay(void)
{
    //glClear (GL_COLOR_BUFFER_BIT);
    if(creation_complete)draw16Squares() ;

    glutSwapBuffers() ;
    glFlush ();
}


void animate()
{
    //angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}
void myInit (void)
{
    glClearColor(1.0, 0.0, 0.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //check
    gluOrtho2D(0.0, theGlutWindowWidth, 0.0, theGlutWindowHeight);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void update(int value)
{

    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);
}

main(int argc, char** argv)
{

    memoryTileGame() ; // code starts to run from here


    //output(0,0,1,1,1,10,"hello");

    glutInit(&argc, argv);
    glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize (theGlutWindowWidth, theGlutWindowHeight); // width
    glutInitWindowPosition (100, 150);
    glutCreateWindow ("My First Attempt");
    //
    glutDisplayFunc(myDisplay);
    //glutDisplayFunc(display);   //display callback function
    glutIdleFunc(animate);      //what you want to do in the idle time (when no drawing is occuring)

    //
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(mouseMotion);
    myInit ();
    glutMainLoop();
}
