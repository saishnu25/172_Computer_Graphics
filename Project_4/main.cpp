// Saishnu Ramesh Kumar (300758706)
// CSCI 172 - Computer Graphics (Project 4)

#include <string.h>
#include <SOIL.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

bool spaceBar = false; // Toggle spacebar to rest the whole scene

GLuint tex; //For Texture Loading
GLfloat rotateModelLR = 0, rotateModelUD = 0; // Rotate left, right, up, down of CUBE (Uses WASD)
GLfloat rotateSkyboxLR = 0, rotateSkyboxUD = 0; // Rotate left, right, up, down of SKYBOX (Uses Arrow keys)
GLfloat cameraLR = 0, cameraUD = 0; // Moves CAMERA left, right, up, down (Uses IJKL)
GLfloat zoomScene = 0; // Zooms CAMERA in and out of scene (Uses ZX)

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.5f, 1.5f, 1.5f, 1.5f };
const GLfloat light_specular[] = { 2.5f, 2.5f, 2.5f, 1.5f };
const GLfloat light_position[] = { 2.0f, 2.0f, 2.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat mat_diffuse[]    = { 1.5f, 1.5f, 1.5f, 1.5f };
const GLfloat mat_specular[]   = { 2.5f, 2.5f, 2.5f, 1.5f };
const GLfloat high_shininess[] = { 50.0f };

/* GLUT callback Handlers */

static void resize(int width, int height)
{
     double Ratio;

   if(width<=height)
            glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
          glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	gluPerspective (75.0f,1,0.1f, 150.0f);
 }


void TextureLoader(char* fileName, GLuint tex)
{
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &tex);

    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, tex); // images are 2D arrays of pixels, bound to the GL_TEXTURE_2D target.
    int width, height; // width & height for the image reader
    unsigned char* image;

    image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image){
        cout << "Image Not Found" << endl;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    // binding image data
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

// -- SKYBOX FUNCTION --
void skybox(){

    // -- SKYBOX RENDERING --
    glPushMatrix();
    glRotatef(rotateSkyboxLR,0,5,0); //Rotate skybox left and right
    glRotatef(rotateSkyboxUD,5,0,0); //Rotate skybox up and down

    // -- SKYBOX TOP --
    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/top.jpg", tex); //Top Image
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0); //Texture and Quad Points for Top
    glTexCoord2f(.25, .333);    glVertex3f(20, 20, 20);
    glTexCoord2f(.5, .333);     glVertex3f(-20, 20, 20);
    glTexCoord2f(.5, 0);        glVertex3f(-20, 20, -20);
    glTexCoord2f(.25, 0);       glVertex3f(20, 20, -20);
    glEnd();

    // -- SKYBOX BOTTOM --
    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/ground.jpg", tex); //Bottom Image
    glBegin(GL_QUADS);
    glNormal3f(0, -1, 0); //Texture and Quad Points for Bottom
    glTexCoord2f(.25, 1);    glVertex3f(20, -20, -20);
    glTexCoord2f(.5, 1);     glVertex3f(-20, -20, -20);
    glTexCoord2f(.5, .667);  glVertex3f(-20, -20, 20);
    glTexCoord2f(.25, .667); glVertex3f(20, -20, 20);
    glEnd();

    // -- SKYBOX FRONT --
    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/sides.jpg", tex); //Side Image
    glBegin(GL_QUADS);
    glNormal3f(0, 0, -1); //Texture and Quad Points for Front
    glTexCoord2f(.25, .667);    glVertex3f(20, -20, 20);
    glTexCoord2f(.5, .667);     glVertex3f(-20, -20, 20);
    glTexCoord2f(.5, .333);     glVertex3f(-20, 20, 20);
    glTexCoord2f(.25, .333);    glVertex3f(20, 20, 20);
    glEnd();

    // -- SKYBOX BACK --
    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/sides.jpg", tex); //Side Image
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1); //Texture and Quad Points for Back
    glTexCoord2f(.75, .667);glVertex3f(-20, -20, -20);
    glTexCoord2f(1, .667); glVertex3f(20, -20, -20);
    glTexCoord2f(1, .333); glVertex3f(20, 20, -20);
    glTexCoord2f(.75, .333); glVertex3f(-20, 20, -20);
    glEnd();

    // -- SKYBOX RIGHT --
    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/sides.jpg", tex); //Side Image
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0); //Texture and Quad Points for Right
    glTexCoord2f(.5, .667);    glVertex3f(-20, -20, 20);
    glTexCoord2f(.75, .667);   glVertex3f(-20, -20, -20);
    glTexCoord2f(.75, .333);   glVertex3f(-20, 20, -20);
    glTexCoord2f(.5, .333);    glVertex3f(-20, 20, 20);
    glEnd();

    // -- SKYBOX LEFT --
    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/sides.jpg", tex); //Side Image
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0); //Texture and Quad Points for Left
    glTexCoord2f(0, .667);      glVertex3f(20, -20, -20);
    glTexCoord2f(.25, .667);    glVertex3f(20, -20, 20);
    glTexCoord2f(.25, .333);    glVertex3f(20, 20, 20);
    glTexCoord2f(0, .333);      glVertex3f(20, 20, -20);
    glEnd();

    glPopMatrix();
}

// -- CUBE FUNCTION --
void cube(){
    glPushMatrix();

    glEnable(GL_TEXTURE_GEN_S); //Texture Coordinator
    glEnable(GL_TEXTURE_GEN_T); //Texture Coordinator

    glRotatef(rotateModelLR, 0, 10, 0); //Rotate cube left and right
    glRotatef(rotateModelUD, 10, 0, 0); //Rotate cube up and down
    glTranslatef(0.0, 0.0, 0.0); //Placed in origin
    glScalef(2, 2, 2); //Scales Cube Size

    glBindTexture(GL_TEXTURE_2D, tex); //Binding image to object
    TextureLoader("image/cube.jpg", tex); //Cube Image
    glutSolidCube(1);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);

    glPopMatrix();
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,10.0,0.0,0.0,0.0,0.0,1.0,0.0); //Camera View

    // your code here
    glRotatef(cameraLR, 0, 5, 0); //Rotate camera left and right
    glRotatef(cameraUD, 5, 0, 0); //Rotate camera up and down
    glTranslated(0, 0, zoomScene); //Zoom in and out of scene

    //If spacebar is pressed, everything gets reset/rests
    if(spaceBar){
        zoomScene = 0;
        cameraLR = 0;
        cameraUD = 0;
        rotateModelLR = 0;
        rotateModelUD = 0;
        rotateSkyboxLR = 0;
        rotateSkyboxUD = 0;
        spaceBar = false;
    }

    skybox(); //Calling Skybox Function
    cube(); //Calling Cube Function

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        //Exit program
        case 27 :
        case 'q':
            exit(0);
        break;

        //Reset camera's scene
        case 'o':
            cameraLR = 0;
            cameraUD = 0;
        break;

        //Spacebar to rest whole scene
        case ' ':
            spaceBar = true;
        break;

        // -- CUBE ROTATE USING WASD --
        //Rotate Up
        case 'w':
            rotateModelUD -= 10.0;
        break;

        //Rotate Left
        case 'a':
            rotateModelLR -= 10.0;
        break;

        //Rotate Down
        case 's':
            rotateModelUD += 10.0;
        break;

        //Rotate Right
        case 'd':
            rotateModelLR += 10.0;
        break;

        // -- SCENE CAMERA USING IJKL --
        //Camera Up
        case 'i':
            cameraUD -= 5.0;
        break;

        //Camera Left
        case 'j':
            cameraLR -= 5.0;
        break;

        //Camera Down
        case 'k':
            cameraUD += 5.0;
        break;

        //Camera Right
        case 'l':
            cameraLR += 5.0;
        break;

        //-- ZOOM SCENE USING ZX --
        //Zoom In
        case 'z':
            zoomScene += 1.0;
        break;

        //Zoom Out
        case 'x':
            zoomScene -= 1.0;
        break;
    }
}

void Specialkeys(int key, int x, int y)
{
    switch(key)
    {
        //-- SKYBOX ROTATE USING ARROW KEYS --
		case GLUT_KEY_LEFT : //Rotate skybox left
			rotateSkyboxLR -= 5.0;
			break;

		case GLUT_KEY_RIGHT : //Rotate skybox right
			rotateSkyboxLR += 5.0;
			break;

		case GLUT_KEY_UP : //Rotate skybox up
			rotateSkyboxUD -= 5.0;
			break;

		case GLUT_KEY_DOWN : //Rotate skybox down
		    rotateSkyboxUD += 5.0;
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
    glutInitWindowPosition(30,30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Skybox (Project #4)");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
