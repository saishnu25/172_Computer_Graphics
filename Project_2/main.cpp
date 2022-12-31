//Saishnu Ramesh Kumar (300758706)
//CSCI 172 - Computer Graphics (Project 2)

#include <string.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <iostream>

#include <math.h>
using namespace std;

bool WireFrame= false;
bool flagC=false, flagT=false, flagS=false; //Flags to read and detect key inputs
GLfloat scaleCz=-3.0, scaleSz=-1.0, scaleTz=2.0; //Variables to zoom along the z axis
GLfloat viewangleTz=0, viewangleCz=0, viewangleSz=0; //Variables to rotate objects
const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

float viewangle = 0;
/* GLUT callback Handlers */
void glutSolidCube ( GLdouble size ); //size 2
void glutSolidTeapot ( GLdouble size ); //size 1.5
void glutSolidSphere ( GLdouble radius, GLint slices, GLint stacks); //Radius 1.3

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (50.0f,1,0.1f, 100.0f);
 }

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, 0, -3);
    gluLookAt(0,5,10,0.0,0.0,0.0,0.0,1.0,0.0);

    if(WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);      //Draw Our Mesh In Wire-frame Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);      //Toggle WIRE FRAME

    // your code here
    //Solid Sphere Code
    glColor3f(0.0,0.0,1.0); //Adding Blue color
    glPushMatrix ();//Places us on the position of the stack
    glRotatef(viewangleSz, 0,1,0);//Rotates the object
    glTranslated(-3.5,0.0,scaleSz);//Moves object along z-axis
    glutSolidSphere (1.3, 24, 24);//Places the object
    glPopMatrix (); //Pops the previous function call

    //Solid Cube Code
    glColor3f(0.0,1.0,0.0); //Adding Green color
    glPushMatrix ();//Places us on the position of the stack
    glRotatef(viewangleCz, 0,1,0);//Rotates the object
    glTranslated(4.0,0.0,scaleCz);//Moves object along z-axis
    glutSolidCube (2.0);//Places the object
    glPopMatrix (); //Pops the previous function call

    //Solid Teapot Code
    glColor3f(1.0,0.0,0.0); //Adding Red color
    glPushMatrix (); //Places us on the position of the stack
    glPushMatrix (); //Places us on the position of the stack
    glRotatef(viewangleTz, 0,1,0); //Rotates the object
    glTranslated(0.0,0.0,scaleTz); //Moves object along z-axis
    glutSolidTeapot (1.5); //Places the object
    glPopMatrix (); //Pops the previous function call

    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q': //Closes the program
            exit(0);
            break;

        case 'c': //If c is pressed, only c is true
            flagC=true;
            flagT=false;
            flagS=false;
            break;

        case 't': //If t is pressed, only t is true
            flagC=false;
            flagT=true;
            flagS=false;
            break;

        case 's': //If s is pressed, only s is true
            flagC=false;
            flagT=false;
            flagS=true;
            break;

        case 'r': //Resets the flags so the user will control the main camera
            flagC=false;
            flagT=false;
            flagS=false;
            WireFrame = false; //This will reset everything including switching off the wireframe
            break;

        case 'w': //Switches to a wireframe style
            flagC = true;
            flagT = true;
            flagS = true;
            WireFrame = true;
            break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {

    case GLUT_KEY_LEFT:
    if(flagC){ //If c is pressed
        viewangleCz-=5; //Changes the variable used to rotate the object
    }

    if(flagT){ //if t is pressed
       viewangleTz-=5;
    }

    if(flagS){ //if s is pressed
        viewangleSz-=5;
    }

    if(flagT==false&&flagC==false&&flagS==false){ //If no buttons have been pressed, goes to main camera controls
        viewangleTz-=5;
        viewangleSz-=5;
        viewangleCz-=5;
    }
    break;

    case GLUT_KEY_RIGHT:

    if(flagC){
        viewangleCz+=5;
    }

    if(flagT){
       viewangleTz+=5;
    }

    if(flagS){
        viewangleSz+=5;
    }

    if(flagT==false&&flagC==false&&flagS==false){
        viewangleTz+=5;
        viewangleSz+=5;
        viewangleCz+=5;
    }
    break;

    case GLUT_KEY_UP:

    if(flagC){//If c is pressed
        scaleCz+=0.5; //Changes the increment variable used to transform the object along the z-axis
    }

    if(flagT){ //if t is pressed
        scaleTz+=0.5;
    }

    if(flagS){ //if s is pressed
        scaleSz+=0.5;
    }

    if(flagT==false&&flagC==false&&flagS==false){ //If no buttons have been pressed, goes to main camera controls
        scaleSz+=0.5;
        scaleTz+=0.5;
        scaleCz+=0.5;
    }

    break;
    case GLUT_KEY_DOWN:

    if(flagC){
        scaleCz-=0.5;
    }

    if(flagT){
        scaleTz-=0.5;
    }

    if(flagS){
        scaleSz-=0.5;
    }

    if(flagT==false&&flagC==false&&flagS==false){
        scaleSz-=0.5;
        scaleTz-=0.5;
        scaleCz-=0.5;
    }


    break;

   }
  glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

static void init(void)
{
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800,600);
    glutInitWindowPosition(200,200);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
