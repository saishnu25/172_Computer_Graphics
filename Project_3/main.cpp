// Saishnu Ramesh Kumar (300758706)
// CSCI 172 - Computer Graphics (Project 3)

#include <SOIL.h>
#include <string.h>
#include <windows.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <unistd.h>
#include <iostream>
using namespace std;

bool WireFrame= false; //To toggle wireframe for each object
bool spaceBar = false; //To toggle spacebar to start the program

GLfloat cameraAngle = 0, cameraZoom = 0; //To rotate left and right and to zoom in and out
GLUquadricObj *sphere = NULL; //Creating a sphere pointer
GLuint tex; //Texture Variable

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.5f, 1.5f, 1.5f, 1.5f };
const GLfloat light_specular[] = { 2.5f, 2.5f, 2.5f, 1.5f };
const GLfloat light_position[] = { 2.0f, 3.0f, 3.0f, 0.0f };

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
	gluPerspective (80.0f,1,0.1f, 150.0f);
}

void Specialkeys(int key, int x, int y)
{

	switch (key) {
		case GLUT_KEY_LEFT : //Rotate camera left
			cameraAngle += 10.0;
			break;

		case GLUT_KEY_RIGHT : //Rotate camera right
			cameraAngle -= 10.0;
			break;

		case GLUT_KEY_UP : //Zoom in with camera
			cameraZoom += 0.5;
			break;

		case GLUT_KEY_DOWN : //Zoom out with camera
		    cameraZoom -= 0.5;
			break;
	}
    glutPostRedisplay();
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

static void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,15,0.1,0.0,0.0,0.0,0.0,1.0,0.0); //Camera View

    if(WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    // your code here
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0; //Speed in which each object goes around its axis
    const double a = t*30.0; //Speed for Earth
    const double b = t*60.0; //Speed for Moon
    const double c = t*40.0; //Speed for Mars

    glBindTexture (GL_TEXTURE_2D, tex); //Binding image to object
    glTranslated(NULL, cameraZoom, 0.0); //To zoom in, and zoom out
    glRotatef(cameraAngle, 0.0, 0.0, 15.0); //To rotate left and right of scene

    //-- Earth --
    glPushMatrix();
    TextureLoader("image/earth.jpg", tex); //Earth Image
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //For lighting
    if(spaceBar){
            glRotated(a, 0, 0, 1);} //Rotating along axis
    glTranslatef(0.0, 0.0, 0.0); //To position object
    gluSphere(sphere, 3.0, 40, 40); //Earth Sphere Object Generated
    glPopMatrix();

    //-- Moon --
    glPushMatrix();
    TextureLoader("image/moon.jpg", tex); //Moon Image
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //For lighting
    if(spaceBar){
            glRotated(b, 0, 0, 1);} //Rotating along axis
    glTranslatef(3.0, 1.25, -5.0); //To position object
    gluSphere(sphere, 1.0, 20, 20); //Moon Sphere Object Generated
    glPopMatrix();

    //-- Mars --
    glPushMatrix();
    TextureLoader("image/mars.jpeg", tex); //Mars Image
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); //For lighting
    if(spaceBar){
            glRotated(c, 0, 0, 1);} //Rotating along axis
    glTranslatef(-3.0, 3.5, -7.0); //To position object
    gluSphere(sphere, 1.5, 25, 25); //Mars Sphere Object Generated
    glPopMatrix();

    glutSwapBuffers();

}

static void key(unsigned char key, int x, int y)
{

    switch (key)
    {
        case 27 :
            case 'q':
                exit(0);
            break;

        case 'w':
            WireFrame =!WireFrame;
        break;

        case ' ': // To detect spacebar input to start program
            spaceBar = true;
        break;
     }
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

    sphere = gluNewQuadric();
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, GL_TRUE);
    gluQuadricNormals(sphere, GLU_SMOOTH);

}

/* Program entry point */

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);

    glutInitWindowSize(800,500);
    glutInitWindowPosition(20,20);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Orbiting Planets (Project #3)");
    init();
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;

}
