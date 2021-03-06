
#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include<math.h>
#include <vector>
#include <iostream>
#include "BmpLoader.h"
using namespace std;


const double PI = 3.14159265389;


/* GLUT callback Handlers */

bool rooftop=1,ct=1;
int anglex= 0, angley = 0, anglez = 0;          //rotation angles
int window;
int wired=0;
int shcpt=1;
int animat = 0;
const int L=20;
const int dgre=3;
int ncpt=L+1;
int clikd=0;
const int nt = 40;				//number of slices along x-direction
const int ntheta = 20;


GLfloat ctrlpoints[33][3] =
{
    { 0.0, 0.0, 0.0}, { -0.3, 0.5, 0.0},
    { 0.1, 1.7, 0.0},{ 0.5, 1.5, 0.0},
    {2.5, 1.2, 0.0},
    { 3.0, 0.0, 0.0}, { 2.7, 0.5, 0.0},
    { 3.1, 1.7, 0.0},{ 3.5, 1.5, 0.0},
    {5.5, 1.2, 0.0},{5.5, 2.2, 0.0},{5.5, 3.2, 0.0},{5.5, 3.2, 0.0},{6.5, 0.2, 0.0},
    {4.5, 0.2, 0.0},{7.5, 1.2, 0.0},{ 3.0, 0.0, 0.0}, { 2.7, 0.5, 0.0},
    { 10.1, 1.7, 0.0},{ 10.5, 1.5, 0.0},
    {12.5, 1.2, 0.0},{12.5, 2.2, 0.0},{12.5, 3.2, 0.0},{12.5, 3.2, 0.0},{13.5, 0.2, 0.0},
    {11.5, 0.2, 0.0},{14.5, 1.2, 0.0}
};

GLfloat ctrlpoints2[L+1][3] =
{

    { 0.0, 1.0, 0.0},{ 0.0, 2.0, 0.0},{ 0.0, 3.0, 0.0},{ 0.0, 4.0, 0.0},{ 0.0, 5.0, 0.0},

    { 4.0, 1.0, 0.0},{ 4.0, 2.0, 0.0},{ 4.0, 3.0, 0.0},{ 4.0, 4.0, 0.0},{ 4.0, 5.0, 0.0},
};

/*
, {1.4, 1.4, 0.0},
    {1.8, 0.4, 0.0},{2.2, 0.4, 0.0},
    {2.6, 1.5, 0.0}, {3.0, 1.4, 0.0},
    {3.4, 1.4, 0.0},{3.8, 1.4, 0.0},
    {4.2, 1.0, 0.0},{4.6, 1.0, 0.0},
    {5.0, 1.0, 0.0},{5.4, 1.0, 0.0},
    {5.8, 0.5, 0.0},{6.2, 0.5, 0.0},
    {6.6, 0.5, 0.0},{7.2, 0.2, 0.0},
    {6.8, 0.52, 0.0}

*/

double ex=0, ey=0, ez=15, lx=0,ly=0,lz=0, hx=0,hy=1,hz=0;

float wcsClkDn[3],wcsClkUp[3];
///////////////////////////////
class point1
{
public:
    point1()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt[2];
int flag=0;
GLint viewport[4]; //var to hold the viewport info
GLdouble modelview[16]; //var to hold the modelview info
GLdouble projection[16]; //var to hold the projection matrix info

//////////////////////////
void scsToWcs(float sx,float sy, float wcsv[3] );
void processMouse(int button, int state, int x, int y);
void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back=0, float ambFactor=1.0, float specFactor=1.0);
///////////////////////////

void scsToWcs(float sx,float sy, float wcsv[3] )
{

    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info

    winX = sx;
    winY = (float)viewport[3] - (float)sy;
    winZ = 0;

    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX;
    wcsv[1]=worldY;
    wcsv[2]=worldZ;


}
void processMouse(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        if(flag!=1)
        {
            flag=1;
            clkpt[0].x=x;
            clkpt[0].y=y;
        }


        scsToWcs(clkpt[0].x,clkpt[0].y,wcsClkDn);
        cout<<"\nD: "<<x<<" "<<y<<" wcs: "<<wcsClkDn[0]<<" "<<wcsClkDn[1];
    }
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if (flag==1)
        {
            clkpt[1].x=x;
            clkpt[1].y=y;
            flag=0;
        }
        float wcs[3];
        scsToWcs(clkpt[1].x,clkpt[1].y,wcsClkUp);
        cout<<"\nU: "<<x<<" "<<y<<" wcs: "<<wcsClkUp[0]<<" "<<wcsClkUp[1];

        clikd=!clikd;
    }
}

//control points
long long nCr(int n, int r)
{
    if(r > n / 2) r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;

        for(int i=0; i<=L; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints[i][0];
        y+=coef*ctrlpoints[i][1];

    }


//    else
//    {
//        for(int i=0; i<=L; i++)
//    {
//        int ncr=nCr(L,i);
//        double oneMinusTpow=pow(1-t,double(L-i));
//        double tPow=pow(t,double(i));
//        double coef=oneMinusTpow*tPow*ncr;
//        x+=coef*ctrlpoints2[i][0];
//        y+=coef*ctrlpoints2[i][1];
//
//    }
//    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

void BezierCurve2( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;

        for(int i=0; i<=20; i++)
    {
        int ncr=nCr(L,i);
        double oneMinusTpow=pow(1-t,double(L-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints2[i][0];
        y+=coef*ctrlpoints2[i][1];

    }


//    else
//    {
//        for(int i=0; i<=L; i++)
//    {
//        int ncr=nCr(L,i);
//        double oneMinusTpow=pow(1-t,double(L-i));
//        double tPow=pow(t,double(i));
//        double coef=oneMinusTpow*tPow*ncr;
//        x+=coef*ctrlpoints2[i][0];
//        y+=coef*ctrlpoints2[i][1];
//
//    }
//    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

///////////////////////
void setNormal(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(-Nx,-Ny,-Nz);
}

void bottleBezier()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    float startx = 0, endx;
    endx = ctrlpoints[L][0];
    //else endx = ctrlpoints2[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}

void bottleBezier2()
{
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;
    float startx = 0, endx;
    endx = ctrlpoints2[10][0];
    //else endx = ctrlpoints2[L][0];
    //number of angular slices
    const float dx = (endx - startx) / nt;	//x step size
    const float dtheta = 2*PI / ntheta;		//angular step size

    float t=0;
    float dt=1.0/nt;
    float xy[2];
    BezierCurve2( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve2( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = r * cosa;
            y1 = r1 * cosa;	//current and next y
            z = r * sina;
            z1 = r1 * sina;	//current and next z

            //edge from point at x to point at next x
            glVertex3f (x, y, z);

            if(j>0)
            {
                setNormal(p1x,p1y,p1z,p2x,p2y,p2z,x, y, z);
            }
            else
            {
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            }
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}


static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    // gluPerspective(60,ar, 2.0, 100.0);

    glOrtho(-8.0, 8.0, -8.0*(GLfloat)height/(GLfloat)width, 8.0*(GLfloat)height/(GLfloat)width, 2.0, 25.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
    gluLookAt(ex,ey,ez,lx,ly,lz,hx,hy,hz);

}


void showControlPoints()
{
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L; i++)
        glVertex3fv(&ctrlpoints[i][0]);
    glEnd();
}
const int width = 1000;
const int height = 1000;

GLfloat eyeX = 0;
GLfloat eyeY = 6;
GLfloat eyeZ = -13;

GLfloat lookX = 0;
GLfloat lookY = 5;
GLfloat lookZ = 0;

GLfloat hookX = 0;
GLfloat hookY = 1;
GLfloat hookZ = 0;

GLfloat alpha = 0.0, theta = 0.0, axis_x=0.0, axis_y=0.0, f_spin=0.0;
GLboolean bRotate = false, uRotate = false;

float da[8]= {0,0,0,0,0,1,1,1};
bool l_on=true;
bool press11 = false,press12=false,press13=false;
bool press21=false,press22=false,press23=false;
bool press41 = false,press42=false,press43=false;
bool flagRot = false;
float rot = 0;
unsigned int ID;

double spin=1.0;
bool isrot=1;

float px=0,py=30,pz=30,spot_cutoff = 30;

static GLfloat v_cube[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte c_ind[6][4] =
{
    {3,1,5,7},  //front
    {6,4,0,2},  //back
    {2,3,7,6},  //top
    {1,0,4,5},  //bottom
    {7,5,4,6},  //right
    {2,0,1,3}   //left
};




static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

void set_material(float R=0.5, float G=0.5, float B=0.5, bool e=false, bool l = false)
{
    GLfloat mat_no[] = {0,0,0, 1.0};
    GLfloat mat_amb[] = {R,G,B,1};
    GLfloat mat_diff[] = {R,G,B,1};
    GLfloat mat_spec[] = {1,1,1,1};
    GLfloat mat_sh[] = {30};

    GLfloat mat_em[] = {1,1,1,1};

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_sh);

    if(e && l)
    {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_em);
    }
    else
    {
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_no);
    }
}


void cube2(float R=0.5, float G=0.5, float B=0.5, bool e=false, float alpha=1)
{

    GLfloat m_no[] = {0, 0, 0, 1.0};
    GLfloat m_amb[] = {R,G,B,1};
    GLfloat m_diff[] = {R,G,B,1};
    GLfloat m_spec[] = {1,1,1,1};
    GLfloat m_sh[] = {30};

    GLfloat m_em[] = {1,1,1,1};

    glMaterialfv(GL_FRONT, GL_AMBIENT, m_amb);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, m_diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, m_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, m_sh);

    if(e & l_on)
        glMaterialfv(GL_FRONT, GL_EMISSION, m_em);
    else
        glMaterialfv(GL_FRONT, GL_EMISSION, m_no);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        for (GLint j=0; j<4; j++)
        {
            glVertex3fv(&v_cube[c_ind[i][j]][0]);
        }
    }
    glEnd();
}

void cube(float R=0.5, float G=0.5, float B=0.5, bool e=false, bool l = false)
{

    set_material(R,G,B,l);

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        getNormal3p(v_cube[c_ind[i][0]][0], v_cube[c_ind[i][0]][1], v_cube[c_ind[i][0]][2],
                    v_cube[c_ind[i][1]][0], v_cube[c_ind[i][1]][1], v_cube[c_ind[i][1]][2],
                    v_cube[c_ind[i][2]][0], v_cube[c_ind[i][2]][1], v_cube[c_ind[i][2]][2]);

        glTexCoord2f(0,1);
        glVertex3fv(&v_cube[c_ind[i][0]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&v_cube[c_ind[i][1]][0]);
        glTexCoord2f(1,0);
        glVertex3fv(&v_cube[c_ind[i][2]][0]);
        glTexCoord2f(1,1);
        glVertex3fv(&v_cube[c_ind[i][3]][0]);
    }
    glEnd();
}

void axes()
{
    float length = 10;
    float width = 0.3;

    // X-axis
    glPushMatrix();
    glTranslatef(length/2,0,0);
    glScalef(length,width,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.8,0.1,0.1);
    glPopMatrix();

    // Y-axis
    glPushMatrix();
    glTranslatef(0,length/2,0);
    glScalef(width,length,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.8,0.1);
    glPopMatrix();

    // Z-axis
    glPushMatrix();
    glTranslatef(0,0,length/2);
    glScalef(width,width,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.1,0.8);
    glPopMatrix();
}

void rodshape1(float x,float y,float z)
{
    float t_length = 6;
    float t_width = 0.5;

    glPushMatrix();
    //glTranslatef(-14.5,7,0);
    glTranslatef(x,y,z);

    glPushMatrix();
    glScalef(t_width,t_width,t_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1,true,true);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,t_length/2+t_width/2);
    glScalef(t_width,t_width,t_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-t_length/2-t_width/2);
    glScalef(t_width,t_width,t_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPopMatrix();

}

void rodshape2(float x, float y, float z)
{
    float t_length = 6;
    float t_width = 0.5;

    glPushMatrix();
    //glTranslatef(-14.5,7,0);
    glTranslatef(x,y,z);

    glPushMatrix();
    glScalef(t_width,t_width,t_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1,true,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,t_length/2+t_width/2);
    glScalef(t_width,t_width,t_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,-t_length/2-t_width/2);
    glScalef(t_width,t_width,t_width);
    glTranslatef(-0.5,0,-0.5);
    cube(0.2,0.2,0.2);
    glPopMatrix();

    glPopMatrix();

}

void spotlightshape(float x,float y, float z)
{
    float s_height = 0.25;
    float s_width = 2;
    float s_length = 2;

    glPushMatrix();
    glTranslatef(x,y,z);
    /*
    glPushMatrix();
    glScalef(s_width,s_height,s_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1);
    glPopMatrix();
    */
    glPushMatrix();
    glScalef(s_width,s_height,s_length);
    glTranslatef(-0.5,0,-0.5);
    cube(1,1,1,true,1);
    glPopMatrix();

    glPopMatrix();

}


void door(float x,float y,float z,int id,float sx=5,float sy=10)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,19);
    glPushMatrix();

    if(da[id]==0)
    {
        glTranslatef(x,y,z);
        //glRotatef(da[id],0,1,0);
        glScalef(sx,sy,0.2);
    }
    else
    {

        glTranslatef(x+(sx/2.0),y,z+(sx/2.0));
        glScalef(0.2,sy,sx);
    }
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


void light1(float x,float y,float z)
{
    //light
    GLfloat l1_no[] = {0, 0, 0, 1.0};
    GLfloat l1_amb[] = {1,0,0, 1.0};
    GLfloat l1_dif[] = {1,0,0,1};
    GLfloat l1_spec[] = {0,0,0,1};
    GLfloat l1_pos[] = {x,y,z,0};
    //GLfloat l_pos[] = {-14.5,-7,10,1.0};
//    glPushMatrix();
//    glTranslatef(x,y,z);
//    glScalef(10,10,10);
//    cube();
//    glPopMatrix();



    glEnable(GL_LIGHT0);

    if(press11)
    {
        glLightfv(GL_LIGHT0, GL_AMBIENT, l1_amb);
    }
    else
    {
        glLightfv(GL_LIGHT0, GL_AMBIENT, l1_no);
    }
    if(press12)
    {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l1_dif);
    }
    else
    {
        glLightfv(GL_LIGHT0, GL_DIFFUSE, l1_no);
    }
    if(press13)
    {
        glLightfv(GL_LIGHT0, GL_SPECULAR, l1_spec);
    }
    else
    {
        glLightfv(GL_LIGHT0, GL_SPECULAR, l1_no);
    }


    glLightfv(GL_LIGHT0, GL_POSITION, l1_pos);
}

void light2(float x,float y, float z)
{
    //light
    GLfloat l2_no[] = {0, 0, 0, 1.0};
    GLfloat l2_amb[] = {0,0,1, 1.0};
    GLfloat l2_dif[] = {0,0,1,1};
    GLfloat l2_spec[] = {0,0,1,1};
    //GLfloat l_pos[] = {-14.5,7,0,1.0};
    GLfloat l2_pos[] = {x,y,z,0};

    glEnable(GL_LIGHT1);

    if(press21)
    {
        glLightfv(GL_LIGHT1, GL_AMBIENT, l2_amb);
    }
    else
    {
        glLightfv(GL_LIGHT1, GL_AMBIENT, l2_no);
    }
    if(press22)
    {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, l2_dif);
    }
    else
    {
        glLightfv(GL_LIGHT1, GL_DIFFUSE, l2_no);
    }
    if(press23)
    {
        glLightfv(GL_LIGHT1, GL_SPECULAR, l2_spec);
    }
    else
    {
        glLightfv(GL_LIGHT1, GL_SPECULAR, l2_no);
    }

    glLightfv(GL_LIGHT1, GL_POSITION, l2_pos);
}

void light3(float x,float y, float z)
{
    //light
    GLfloat l3_no[] = {0, 0, 0, 1.0};
    GLfloat l3_amb[] = {1,0,1, 1.0};
    GLfloat l3_dif[] = {1,0,1,1};
    GLfloat l3_spec[] = {1,0,1,1};
    GLfloat l3_pos[] = {x,y,z,1.0};
    //GLfloat l_pos[] = {14,-7,10,1.0};

    glEnable(GL_LIGHT2);

    if(press41)
    {
        glLightfv(GL_LIGHT2, GL_AMBIENT, l3_amb);
    }
    else
    {
        glLightfv(GL_LIGHT2, GL_AMBIENT, l3_no);
    }
    if(press42)
    {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, l3_dif);
    }
    else
    {
        glLightfv(GL_LIGHT2, GL_DIFFUSE, l3_no);
    }
    if(press43)
    {
        glLightfv(GL_LIGHT2, GL_SPECULAR, l3_spec);
    }
    else
    {
        glLightfv(GL_LIGHT2, GL_SPECULAR, l3_no);
    }

    glLightfv(GL_LIGHT2, GL_POSITION, l3_pos);

    // spot light extra
    GLfloat l3_spt[] = {0,0,-1,1};
    GLfloat spt_ct[] = {100};
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, l3_spt);
    glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, spt_ct);

}
void DrawCircle(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    for(int ii = 0; ii < num_segments; ii++)
    {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component

        glVertex2f(x + cx, y + cy);//output vertex

    }
    glEnd();
}


static GLfloat v_pyramid[5][3] =
{
    {0.0, 0.0, 0.0},
    {0.0, 0.0, 2.0},
    {2.0, 0.0, 2.0},
    {2.0, 0.0, 0.0},
    {1.0, 4.0, 1.0}
};

static GLubyte p_Indices[4][3] =
{
    {4, 1, 2},
    {4, 2, 3},
    {4, 3, 0},
    {4, 0, 1}
};

static GLubyte quadIndices[1][4] =
{
    {0, 3, 2, 1}
};

static GLfloat colors[5][3] =
{
    {0.0, 0.0, 1.0},
    {0.5, 0.0, 1.0},
    {0.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {0.8, 0.0, 0.0}
};

void drawpyramid()
{
    //glDisable(GL_LIGHTING);
    //glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);
    for (GLint i = 0; i <4; i++)
    {
        //glColor3f(colors[i][0],colors[i][1],colors[i][2]);
        getNormal3p(v_pyramid[p_Indices[i][0]][0], v_pyramid[p_Indices[i][0]][1], v_pyramid[p_Indices[i][0]][2],
                    v_pyramid[p_Indices[i][1]][0], v_pyramid[p_Indices[i][1]][1], v_pyramid[p_Indices[i][1]][2],
                    v_pyramid[p_Indices[i][2]][0], v_pyramid[p_Indices[i][2]][1], v_pyramid[p_Indices[i][2]][2]);

        glVertex3fv(&v_pyramid[p_Indices[i][0]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][1]][0]);
        glVertex3fv(&v_pyramid[p_Indices[i][2]][0]);
    }
    glEnd();

    glBegin(GL_QUADS);
    for (GLint i = 0; i <1; i++)
    {
        //glColor3f(colors[4][0],colors[4][1],colors[4][2]);
        getNormal3p(v_pyramid[quadIndices[i][0]][0], v_pyramid[quadIndices[i][0]][1], v_pyramid[quadIndices[i][0]][2],
                    v_pyramid[quadIndices[i][1]][0], v_pyramid[quadIndices[i][1]][1], v_pyramid[quadIndices[i][1]][2],
                    v_pyramid[quadIndices[i][2]][0], v_pyramid[quadIndices[i][2]][1], v_pyramid[quadIndices[i][2]][2]);

        glVertex3fv(&v_pyramid[quadIndices[i][0]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][1]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][2]][0]);
        glVertex3fv(&v_pyramid[quadIndices[i][3]][0]);
    }
    glEnd();


}


void building()
{
    ///Building Front Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(-25,0,35);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(-20,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(-30,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(-10,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(-40,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);




    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-25,0,35.1);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Building Front Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    //glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(25,0,35);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(20,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(30,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(10,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,23);
    glPushMatrix();
    glTranslatef(40,10,35);
    glScalef(5,5,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.6,0.8,0.3);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(25,0,35.1);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Building Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(-45,0,75);
    glScalef(0.1,40,80);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-44.9,0,75);
    glScalef(0.1,40,80);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Building Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(45,0,75);
    glScalef(0.1,40,80);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(44.9,0,75);
    glScalef(0.1,40,80);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Building Back Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(25,0,115);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(25,0,114.9);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Building Back Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(-25,0,115);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-25,0,114.9);
    glScalef(40,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Inside Front Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(5,0,45);
    glScalef(0.1,40,20);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Inside Front Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-5,0,45);
    glScalef(0.1,40,20);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Inside Back Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(5,0,105);
    glScalef(0.1,40,20);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Inside Back Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-5,0,105);
    glScalef(0.1,40,20);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Inside Front Left2
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-27.5,0,55);
    glScalef(35,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    door(-7.5,7.5,55,1,5,25);


    ///Inside Front Right2
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(27.5,0,55);
    glScalef(35,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    door(7.5,7.5,55,2,5,25);

    ///Inside Back Left2
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-27.5,0,95);
    glScalef(35,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    door(-7.5,7.5,95,3,5,25);

    ///Inside Back Right2
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(27.5,0,95);
    glScalef(35,40,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    door(7.5,7.5,95,4,5,25);

    ///Inside Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(-25,0,72.5);
    glScalef(0.1,40,35);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    door(-27.5,7.5,90,5,5,25);
    glPopMatrix();

    ///Inside Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(25,0,72.5);
    glScalef(0.1,40,35);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    door(22.5,7.5,90,6,5,25);
    glPopMatrix();

}
void car()
{

    glEnable(GL_LIGHTING);
    //whole car
    glPushMatrix();
    //glRotatef(90,0,1,0);
    glTranslatef(5,0.5,20);
    //body

    glPushMatrix();
    glTranslatef(0,1.5,0);
    glScalef(8,2,3);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0.9,0.9,0.9);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();


    //top
    glPushMatrix();
    glTranslatef(0,3.5,0);
    glScalef(4,2,3);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0.7569,0.7569,0.847);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();

    //glass
    glPushMatrix();
    glTranslatef(0.95,3.45,1.5);
    glScalef(1.8,1.8,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0,0,0);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.95,3.45,1.5);
    glScalef(1.8,1.8,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0,0,0);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.95,3.45,-1.5);
    glScalef(1.8,1.8,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0,0,0);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.95,3.45,-1.5);
    glScalef(1.8,1.8,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0,0,0);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();

    //front glass

    glPushMatrix();
    glTranslatef(2,3.45,0);
    glScalef(0.1,1.8,3);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0,0,0);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2,3.45,0);
    glScalef(0.1,1.8,3);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube(0,0,0);
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();


    //frontlight
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glTranslatef(-4,1.45,0);
    glScalef(0.1,1.8,3);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    cube();
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //backlight
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,8);
    glPushMatrix();
    glTranslatef(4,1.45,0);
    glScalef(0.1,1.8,3);
    glTranslatef(-0.5,-0.5,-0.5);
    //cube();
    set_material(0.5,0.5,0.5,true,true);
    cube();
    //cube(0.5804,0.5451,0.5568);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

    glDisable(GL_LIGHTING);

}

void LoadTexture(const char*filename)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void cover()
{

    ///Front
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glTranslatef(0,20,45);
    glScalef(90,0.1,20);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Back
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glTranslatef(0,20,105);
    glScalef(90,0.1,20);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glTranslatef(35,20,75);
    glScalef(20,0.1,40);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,9);
    glPushMatrix();
    glTranslatef(-35,20,75);
    glScalef(20,0.1,40);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void goalpost()
{
    ///Left Post
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(17.5,0,78);
    glScalef(0.3,15,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(17.5,0,72);
    glScalef(0.3,15,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(17.5,7.5,75);
    glRotatef(90,1,0,0);
    glScalef(0.3,6,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    ///Right Post
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-17.5,0,78);
    glScalef(0.3,15,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-17.5,0,72);
    glScalef(0.3,15,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-17.5,7.5,75);
    glRotatef(90,1,0,0);
    glScalef(0.3,6,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void fence()
{
    ///Fence Left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(30,0,25);
    glScalef(50,10,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(55,0,75);
    glScalef(0.5,10,100);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Fence Right
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(-30,0,25);
    glScalef(50,10,0.5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(-55,0,75);
    glScalef(0.5,10,100);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void wardrobe(float x,float y,float z)
{

    ///wardrobe body
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,11);
    glPushMatrix();
    glTranslatef(x,y,z);
    glScalef(10,20,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///wardrobe front
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,12);
    glPushMatrix();
    glTranslatef(x+5,y,z);
    glScalef(0.1,20,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void board()
{

    glPushMatrix();
//    glColorMaterial ( GL_FRONT_AND_BACK, GL_EMISSION ) ;
//    glEnable ( GL_COLOR_MATERIAL ) ;
//    glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,0);
    glEnable(GL_LIGHTING);
    glTranslatef(5.1,15,48);
    glScalef(0.1,5,10);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0,0,0,1,1);
    glDisable(GL_LIGHTING);
    glPopMatrix();

}
void clock()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,14);
    glPushMatrix();
    glTranslatef(15,4,44);
    ///Clock
    glPushMatrix();
    glTranslatef(5.3,10.5,9.6);
    glScalef(4,4,0.1);
    cube(1,1,1);
    glPopMatrix();

    ///Circle
    glPushMatrix();
    glTranslatef(0,5,10);
    glScalef(15,15,15);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
void fan()
{
    float height = 10;

    glPushMatrix();
    glEnable(GL_LIGHTING);
    glTranslatef(25,10,45);
    //glScalef(10,5,10);
//    for(int i=0; i<=360 ; i=i+10)
//    {
//
//        glPushMatrix();
//        glTranslatef(0,height,0);
//        glRotatef(i,0,1,0);
//        glScalef(5,2.5/8,2.5);
//        glTranslatef(-0.5,0,-0.5);
//        cube(0.502, 0.000, 0.502);
//        glPopMatrix();
//
//    }

    glPushMatrix();
    glTranslatef(0,height-5-2.5/8,0);
    glScalef(1,4,1);
    glTranslatef(-0.5,0,-0.5);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glRotatef(spin,0,1,0);
    for(int i=0; i<=360 ; i=i+10)
    {

        glPushMatrix();
        glTranslatef(0,height-5-2.5/8-2.5,0);
        glRotatef(i,0,1,0);
        glScalef(5,2.5,2.5);
        glTranslatef(-0.5,0,-0.5);
        cube(1.000, 1.000, 0.000);
        glPopMatrix();

    }

    glPushMatrix();
    glTranslatef(0,height-5-2.5/8-2.5+1,0);
    glScalef(15,0.5,2.5);
    glTranslatef(-0.5,0,-0.5);
    cube(0, 0.000, 0.502);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,height-5-2.5/8-2.5+1,0);
    glScalef(2.5,0.5,15);
    glTranslatef(-0.5,0,-0.5);
    cube(0, 0.000, 0.502);

    glPopMatrix();

    glPopMatrix();
    glDisable(GL_LIGHTING);
    glPopMatrix();
}

void table(float x,float y,float z, unsigned int id=11)
{
    float height=6;
    float width=5;
    float length=5;

    float base_height=1;
    float leg_height=height-base_height;
    float leg_width=0.5;

    // whole table
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,id);
    glPushMatrix();
    glTranslatef(x,y,z);

    // base
    glPushMatrix();
    glScalef(width,base_height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    // legs
    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}


void chair(float x,float y,float z,int turned=0)
{
    float height=3;
    float width=3;
    float length=3;

    float base_height=1;
    float leg_height=height-base_height;
    float leg_width=0.5;

    // whole chair
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,13);
    glPushMatrix();
    glTranslatef(x,y,z);
    if(!turned)
        glRotatef(180,0,1,0);
    else if(turned==3)
        glRotatef(90,0,1,0);
    else if(turned==2)
        glRotatef(270,0,1,0);

    // base
    glPushMatrix();
    glScalef(width,base_height-0.5,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    //back
    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,0);
    glScalef(0.5,height,length);
    glTranslatef(-0.5,0,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    // legs
    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-(width/2-leg_width/2),0,-(length/2-leg_width/2));
    glScalef(leg_width,leg_height,leg_width);
    glTranslatef(-0.5,-1,-0.5);
    cube(0.53,0.39,0.28);
    glPopMatrix();

    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void tv()
{

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-26,13,70);
    glScalef(1,5,5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(-26,13,69);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-26,13,75);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-26,12,70);
    glScalef(1,1,6);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-26,18,70);
    glScalef(1,1,6);
    cube(0,0,0);
    glPopMatrix();
    glDisable(GL_LIGHTING);
}
void carrom()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,15);
    glPushMatrix();
    glTranslatef(30,7,65);
    glScalef(5,1,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glPushMatrix();
    glTranslatef(1,0,0);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(31.5,7.1,65);
    glScalef(0.5,1,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0,0,0);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(26.5,7.1,65);
    glScalef(0.5,1,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0,0,0);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(29,7.1,67.5);
    glRotatef(90,0,1,0);
    glScalef(0.5,1,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0,0,0);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(29,7.1,62.5);
    glRotatef(90,0,1,0);
    glScalef(0.5,1,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0,0,0);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glPopMatrix();
    ///Carrom balls
    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(30,7.2,64);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(29,7.2,64.5);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(28,7.2,65);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(27.5,7.2,65.5);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();
    glDisable(GL_LIGHTING);


    glPushMatrix();
    glTranslatef(30,7.2,66);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(29,7.2,66.5);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(28,7.2,63);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(28,7.2,63.5);
    glRotatef(90,1,1,0);
    glScalef(1,1,1);
    DrawCircle(0.5, 0.5, 0.2, 100);
    glPopMatrix();


    table(30,6,65);
}

void pingpong()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,13);
    glPushMatrix();
    glTranslatef(38,7,65);
    glScalef(5,1,5);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    glPushMatrix();
    glTranslatef(40.5,8,65);
    glScalef(0.3,1.5,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(35.5,8,65);
    glScalef(0.3,1.5,0.3);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,18);
    glPushMatrix();
    glTranslatef(38,8,65);
    glScalef(4.5,1.5,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    table(38,6,65);
}


void dart()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,17);
    glPushMatrix();
    glTranslatef(35,15,94);
    glScalef(6,6,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}
void chess()
{
    table(38,6,75);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,16);
    glPushMatrix();
    glTranslatef(38,7,75);
    glScalef(4,1,4);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    //glRotatef(90,0,1,0);
    chair(36,6,75,1);
    chair(40,6,75);
    glPopMatrix();
}



void classroom1(int id)
{

    if(id==2)
    {
        wardrobe(11,7.5,48);
        //chairtable();
        table(12,6,39);
        chair(10,6,39,1);
    }
    else
    {
        wardrobe(11,7.5,39);
        //chairtable();
        table(12,6,48);
        chair(10,6,48,1);
    }

    board();
    clock();
    fan();



    table(20,6,38);
    chair(22,6,38);
    table(20,6,45);
    chair(22,6,45);
    table(20,6,52);
    chair(22,6,52);

    table(30,6,38);
    chair(32,6,38);
    table(30,6,45);
    chair(32,6,45);
    table(30,6,52);
    chair(32,6,52);

    table(40,6,38);
    chair(42,6,38);
    table(40,6,45);
    chair(42,6,45);
    table(40,6,52);
    chair(42,6,52);

}

void lab()
{

    glEnable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(-40,7,38);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-42,7,38);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-40,7,45);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-42,7,45);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-40,7,52);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-42,7,52);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,7,38);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-32,7,38);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,7,45);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-32,7,45);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-30,7,52);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-32,7,52);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,7,38);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-22,7,38);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,7,45);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-22,7,45);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-20,7,52);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-22,7,52);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();



    glPushMatrix();
    glTranslatef(-10,7,38);
    glScalef(0.2,2,2);
    cube(0,0,0);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-10,7,38);
    glScalef(2,0.2,2);
    cube(0,0,0);
    glPopMatrix();

    glDisable(GL_LIGHTING);

    table(-40,6,38);
    chair(-42,6,38,1);
    table(-40,6,45);
    chair(-42,6,45,1);
    table(-40,6,52);
    chair(-42,6,52,1);

    table(-30,6,38);
    chair(-32,6,38,1);
    table(-30,6,45);
    chair(-32,6,45,1);
    table(-30,6,52);
    chair(-32,6,52,1);

    table(-20,6,38);
    chair(-22,6,38,1);
    table(-20,6,45);
    chair(-22,6,45,1);
    table(-20,6,52);
    chair(-22,6,52,1);

    table(-10,6,38);
    chair(-7,6,38);

    ///projector
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-6,18,38);
    glScalef(1,0.5,15);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-6,10,38);
    glScalef(1,8,15);
    cube();
    glPopMatrix();

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,10);
    glPushMatrix();
    glTranslatef(-15,15,45);
    glScalef(1,0.5,10);
    cube();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glPushMatrix();
    glTranslatef(-14,15,47.5);
    glScalef(3,1,3);
    cube();
    glPopMatrix();
}
void sports()
{
    carrom();
    pingpong();
    chess();
    dart();

}

void trees()
{


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(10,5,10);
    glScalef(2,2,2);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(10,5,20);
    glScalef(2,2,2);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(10,5,-10);
    glScalef(2,2,2);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(-12,5,10);
    glScalef(2,2,2);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(-12,5,20);
    glScalef(2,2,2);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glTranslatef(-12,5,-10);
    glScalef(2,2,2);
    drawpyramid();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    glEnable(GL_LIGHTING);

    glPushMatrix();
    glTranslatef(10,0,20);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10,0,10);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10,0,-10);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10,0,20);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10,0,10);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-10,0,-10);
    glScalef(1,5,1);
    cube(0,0,0);
    glPopMatrix();

    glDisable(GL_LIGHTING);
}

void teachers()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(-40,6,66);
    glScalef(10,1,26);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    glScalef(2,1,5);
    table(-17.5,5,15.8);
    glPopMatrix();

    chair(-28,5,70);
    chair(-28,5,75);
    chair(-28,5,80);
    chair(-28,5,85);
    chair(-28,5,90);
    chair(-43,5,70,1);
    chair(-43,5,75,1);
    chair(-43,5,80,1);
    chair(-43,5,85,1);
    chair(-43,5,90,1);


    //glRotatef(90,0,1,0);
    chair(-35,5,65,2);
    chair(-35,5,92,3);
    wardrobe(-35,7.5,60);
    tv();

}

void curved_trees(float x,float y,float z)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    //glDisable(GL_LIGHTING);
    glTranslatef(x,y,z);
    glScalef(2,2,2);
    //if(animat)
        //glRotated(a,0,0,1);
    //else
        //glRotated(135,0,0,1);

    glRotatef( anglex, 1.0, 0.0, 0.0);
    glRotatef( angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
    glRotatef( anglez, 0.0, 0.0, 1.0);
    glScaled(0.5,0.5,0.5);
    glRotatef( 270, 0.0, 0.0, 1.0);
    glTranslated(-3.5,0,0);
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    //matColor(0.9,0.5,0.1,20);   // front face color
    //matColor(0.0,0.5,0.8,20,1);  // back face color


    bottleBezier();


    if(shcpt)
    {
        //matColor(0.0,0.0,0.9,20);
        //showControlPoints();
    }

    glPopMatrix();
    //axes();
    glDisable(GL_TEXTURE_2D);
}


void weight(float x,float y,float z)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,22);

    glPushMatrix();
    //glDisable(GL_LIGHTING);
    glTranslatef(x,y,z);
    glScalef(2,2,2);
    //if(animat)
        //glRotated(a,0,0,1);
    //else
        //glRotated(135,0,0,1);

    glRotatef( anglex, 1.0, 0.0, 0.0);
    glRotatef( angley, 0.0, 1.0, 0.0);         	//rotate about y-axis
    glRotatef( anglez, 0.0, 0.0, 1.0);
    glScaled(0.5,0.5,0.5);
    glRotatef( 180, 0.0, 0.0, 1.0);
    glTranslated(-3.5,0,0);
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info

    //matColor(0.9,0.5,0.1,20);   // front face color
    //matColor(0.0,0.5,0.8,20,1);  // back face color


    bottleBezier2();


    if(shcpt)
    {
        //matColor(0.0,0.0,0.9,20);
        //showControlPoints();
    }

    glPopMatrix();
    //axes();
    glDisable(GL_TEXTURE_2D);
}
static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-3, 3, -3, 3, 2.0, 100.0);




    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, hookX,hookY,hookZ);
    //glViewport(0, 0, width, height);

    glRotatef(rot, 0,1,0);
    const double t = glutGet(GLUT_ELAPSED_TIME) / 5000.0;
    const double a = t*90.0;
    if(wired)
    {
        glPolygonMode( GL_FRONT, GL_LINE ) ;
        glPolygonMode( GL_BACK, GL_LINE ) ;

    }
    else
    {
        glPolygonMode( GL_FRONT,GL_FILL ) ;
        glPolygonMode( GL_BACK, GL_FILL ) ;
    }
    //glPushMatrix();
    glEnable(GL_LIGHTING);
    glPushMatrix();
    curved_trees(10,8,0);
    curved_trees(-10,8,0);
    glPopMatrix();


    if(wired)
    {
        glPolygonMode( GL_FRONT, GL_LINE ) ;
        glPolygonMode( GL_BACK, GL_LINE ) ;

    }
    else
    {
        glPolygonMode( GL_FRONT,GL_FILL ) ;
        glPolygonMode( GL_BACK, GL_FILL ) ;
    }


    glPushMatrix();
    glTranslatef(32.5,5,90);
    glScalef(10,0.5,0.5);
    cube(1,1,1);
    glPopMatrix();

    weight(40,5,90);
    weight(30,5,90);
    glPopMatrix();

//    glEnable(GL_LIGHTING);
//    glPushMatrix();
//    glTranslatef(8,0,10);
//    glScalef(0.5,10,0.5);
//    cube(0,0,0);
//    glPopMatrix();
//    glDisable(GL_LIGHTING);

    //cube(0.5,0.5,0.5,1,1);
    ///classrooms
    classroom1(0);
    glPushMatrix();
    //glEnable(GL_LIGHTING);
    glTranslatef(0,0,60);

    classroom1(2);
    glRotatef(180,0,1,0);
    glPopMatrix();

    ///Main Door
    glPushMatrix();
    //glScalef(10,20,1);
    door(0,5,35,0,10,20);
    glPopMatrix();

    rodshape1(44,15,45);
    rodshape2(44,15,105);
    spotlightshape(-15,15,45);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    light1(44,15,45);
    //light1(8,0,10);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    light2(40,15,105);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    glEnable(GL_LIGHTING);
    glPushMatrix();
    light3(-15,15,45);
    glPopMatrix();
    glDisable(GL_LIGHTING);
    //glColor3b(1,1,1);
    //light3();

    ///Labs
    lab();
    glPushMatrix();
    glTranslatef(0,0,60);
    lab();
    glPopMatrix();

    ///Sports Room
    sports();
    ///Cars

    glPushMatrix();
    glTranslatef(15,0,12);

    car();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(15,0,7);
    car();
    glPopMatrix();

    ///Main Road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,1);
    glPushMatrix();
    glScalef(10,0.2,70);

    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Grass
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,2);
    glPushMatrix();
    glScalef(200,0.1,250);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    fence();

    building();

    ///Name plate
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,20);
    glPushMatrix();
    glTranslatef(0,17.5,35);
    glScalef(10,5,1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///back
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,3);
    glPushMatrix();
    glTranslatef(0,10,115);
    glScalef(10,20,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,5);
    glPushMatrix();
    glTranslatef(0,10,114.9);
    glScalef(10,20,0.1);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);


    ///Football Field
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,6);
    glPushMatrix();
    glTranslatef(0,0,75);
    glScalef(40,0.3,30);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Floor
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,7);
    glPushMatrix();
    glTranslatef(0,0,75);
    glScalef(90,0.2,80);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    ///Rooves
    //cover();
    switch(rooftop)
    {
    case 1:
        cover();
        break;

    }

    ///GoalPosts
    goalpost();


    trees();

    teachers();

    //glPopMatrix();
    glutSwapBuffers();



}

void animate()
{


    glutPostRedisplay();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case '/':
        lookX=0;
        lookY=5;
        lookZ=0;
        eyeX=0;
        eyeY=6;
        eyeZ=-13;
        rot=0;
        break;
//    case 'o':
//        eyeX++;
//        lookX++;
//        hookX++;
//        hookY++;
//        hookZ++;
//        break;
    case 'q':
        exit(0);
        break;
    case 'f':
        isrot^=1;
        break;
    case 'w':
        eyeZ++;
        lookZ++;
        break;
    case 's':
        eyeZ--;
        lookZ--;
        break;
    case 'a':
        eyeX++;
        lookX++;
        break;
    case 'd':
        eyeX--;
        lookX--;
        break;
    case 'u':
        eyeY++;
        lookY++;
        break;
    case 'j':
        eyeY--;
        lookY--;
        printf("%f %f %f %f %f %f\n",eyeX,eyeY,eyeZ, lookX,lookY,lookZ);
        break;
    case '.':
        rot--;
        break;
    case ',':
        rot++;
        break;
    case 'i':
        eyeY++;
        break;
    case 'k':
        eyeY--;
        break;
    case 'r':
        rot=rot+90;
        break;
    case 'z':
        press11^=1;
        break;
    case 'Z':
        press12^=1;
        break;
    case 'V':
        press13^=1;
        break;
    case 'x':
        press21^=1;
        break;
    case 'X':
        press22^=1;
        break;
    case 'B':
        press23^=1;
        break;
    case 'c':
        press41^=1;
        break;
    case 'C':
        press42^=1;
        break;
    case 'N':
        press43^=1;
        break;
    case 'v':
        lookX++;
        break;
    case 'b':
        lookX--;
        break;
    case 'n':
        eyeX++;
        break;
    case 'm':
        eyeX--;
        break;
    case '0':
        if(da[0]==0)
            da[0]=1;
        else
            da[0]=0;
        break;
    case '1':
        if(da[1]==0)
            da[1]=1;
        else
            da[1]=0;
        break;
    case '2':
        if(da[2]==0)
            da[2]=1;
        else
            da[2]=0;
        break;
    case '3':
        if(da[3]==0)
            da[3]=1;
        else
            da[3]=0;
        break;
    case '4':
        if(da[4]==0)
            da[4]=1;
        else
            da[4]=0;
        break;
    case '5':
        if(da[5]==0)
            da[5]=1;
        else
            da[5]=0;
        break;
    case '6':
        if(da[6]==0)
            da[6]=1;
        else
            da[6]=0;
        break;
    case 'h':
        rooftop^=1;
        break;
    case 'g':
       eyeX=12.000000, eyeY=14.000000, eyeZ=50.000000, lookX=99.000000, lookY=13.000000, lookZ=63.000000;
       break;
    case 'y':
        eyeX=33.000000, eyeY=12.000000,eyeZ=59.000000, lookX=33.000000, lookY=11.000000, lookZ=72.000000;
        break;
    case 'l':
        eyeX=-37.000000, eyeY=9.000000, eyeZ=61.000000, lookX=-37.000000, lookY=8.000000, lookZ=74.000000;
        break;
    case 'p':
        eyeX=-6.000000, eyeY=11.000000, eyeZ=45.000000, lookX=-43.000000, lookY=10.000000, lookZ=58.000000;
        break;
    }

    glutPostRedisplay();
}

static void idle(void)
{

    if (isrot)
    {
        spin=spin+1.0;

        if(spin > 360)
            spin = 0;
    }
    //if(rooftop)cover();
    glutPostRedisplay();
}


const GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 1.0f };

void matColor(float kdr, float kdg, float kdb,  float shiny, int frnt_Back, float ambFactor, float specFactor)
{

    const GLfloat mat_ambient[]    = { kdr*ambFactor, kdg*ambFactor, kdb*ambFactor, 1.0f };
    const GLfloat mat_diffuse[]    = { kdr, kdg, kdb, 1.0f };
    const GLfloat mat_specular[]   = { 1.0f*specFactor, 1.0f*specFactor, 1.0f*specFactor, 1.0f };
    const GLfloat high_shininess[] = { shiny };
    if(frnt_Back==0)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==1)
    {
        glMaterialfv(GL_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_BACK, GL_SHININESS, high_shininess);
    }
    else if(frnt_Back==2)
    {
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, high_shininess);
    }

}

/* Program entry point */
void myInit()
{
    glClearColor(.1,.1,.1,1);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}


int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);




    glutCreateWindow("1607006");
    //glDisable(GL_LIGHTING);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);

    //glutIdleFunc(animate);
    //glutSpecialFunc(collision);
    glutIdleFunc(idle);

    //glutReshapeFunc(res);

    glClearColor(0.4,0.4,1,1);

    glEnable(GL_DEPTH_TEST);

    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    //glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    printf("ROLL: 1607006 \n PROJECT TITLE: 3D CLASSROOM/SCHOOL PROJECT\n");
    printf(" 'w' to look forward \n 's' to look backward \n 'd' to look right \n 'a' to look left.\n");
    printf(" 'u' to look up \n 'j' to look down \n 'f' to turn on/off fan \n '/' to reset view.\n");
    printf(" '.' to rotate left \n ',' to rotate right \n 'i' eye up \n 'k' eye down\n");
    printf(" 'r' rotate 90 degree \n 'z' light1 on/of \n 'x' light2 on/off \n 'c' light3 on/off\n");
    printf(" 'v' look left \n 'b' look right \n 'n' eye right \n 'm' eye left\n");
    printf(" '0' main door on/off \n '1' lab1 door on/off \n '2' class1 door on/off \n");
    printf(" '3' lab2 door on/off \n '4' class2 door on/off \n '5' teachers' room on/off \n '6' sports room on/off\n 'h' to remove/put rooftop\n");


    LoadTexture("D:\\1607006\\road.bmp");//1
    LoadTexture("D:\\1607006\\grass.bmp");//2
    LoadTexture("D:\\1607006\\fence.bmp");//3
    LoadTexture("D:\\1607006\\window.bmp");//4
    LoadTexture("D:\\1607006\\brick.bmp");//5
    LoadTexture("D:\\1607006\\football.bmp");//6
    LoadTexture("D:\\1607006\\floor.bmp");//7
    LoadTexture("D:\\1607006\\carlight.bmp");//8
    LoadTexture("D:\\1607006\\topview.bmp");//9
    LoadTexture("D:\\1607006\\pipe.bmp");//10
    LoadTexture("D:\\1607006\\wardrobebody.bmp");//11
    LoadTexture("D:\\1607006\\wardrobefront.bmp");//12
    LoadTexture("D:\\1607006\\chair.bmp");//13
    LoadTexture("D:\\1607006\\clock.bmp");//14
    LoadTexture("D:\\1607006\\wardrobebody.bmp");//15
    LoadTexture("D:\\1607006\\chess.bmp");//16
    LoadTexture("D:\\1607006\\dart.bmp");//17
    LoadTexture("D:\\1607006\\net.bmp");//18
    LoadTexture("D:\\1607006\\door.bmp");//19
    LoadTexture("D:\\1607006\\name.bmp");//20
    LoadTexture("D:\\1607006\\tree.bmp");//21
    LoadTexture("D:\\1607006\\flower.bmp");//22
    LoadTexture("D:\\1607006\\win.bmp");//23

    glutMainLoop();

    return EXIT_SUCCESS;
}
