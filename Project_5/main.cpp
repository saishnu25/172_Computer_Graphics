// Saishnu Ramesh Kumar (300758706)
// CSCI 172 - Computer Graphics (Project 5)

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <SOIL.h>
#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>

using namespace std;

const GLfloat light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

GLuint tex[10];
bool spaceBar = false;
bool WireFrame = false;
bool hasVT = false; //Checks if VT exists

GLfloat moveLeftRight = 0, moveUpDown = 0; //Skybox movement using arrow keys
GLfloat movement = 0; //Zoom starting point
GLfloat rotateX = 0, rotateY = 0, rotateZ = 0; //Rotating points
GLfloat thetaX = 0, thetaY = 0; //Mouse movement along x and y axis

typedef struct Vec3 //v && vn && face
{
    float x; //The x-axis
    float y; //The y-axis
    float z; //The z-axis
}Vec3; //Three for each coords

typedef struct Vec2 //vt
{
    float x; //x-axis
    float y; //y-axis
}Vec2; //vt only has two points

typedef struct Vec4 // face quad
{
    int ver1, ver2, ver3, ver4, vext1, vext2, vext3, vext4, vnorm1, vnorm2, vnorm3, vnorm4;
    int count;
}Vec4; //For the face that has a quad

int ver = 10; //The ver stands for vertex
int verT = 10; //The verT stands for vertex Texture
int face = 10; //For face
int verN = 10; //The verN stands for vertex Normals

Vec3* normals = (Vec3*)malloc(verN * sizeof(Vec3));
Vec2* textureV = (Vec2*)malloc(verT * sizeof(Vec2));
Vec3* vertexs = (Vec3*)malloc(ver * sizeof(Vec3));
Vec4* faces = (Vec4*)malloc(face * sizeof(Vec4));
int cow, trunk, ele, ven;

/* GLUT callback Handlers */
static void resize(int width, int height)
{
    double Ratio;
    if (width <= height)
        glViewport(0, (GLsizei)(height - width) / 2, (GLsizei)width, (GLsizei)width);
    else
        glViewport((GLsizei)(width - height) / 2, 0, (GLsizei)height, (GLsizei)height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, 1, 0.1f, 200.0f);
}

//-- Load Object Function --
int loadObject(char* filename)
{
    int a = 1, b = 1, c = 1, f = 0, faceAmount = 0;
    ifstream file;
    file.open(filename); //Opens the file
    string line;
    if (!file.is_open())
    {
        cout << "File not opened!";
        return -1;
    }
    while (getline(file, line)) //While getting each line from the file it will enter the function.
    {
        string prefix = line.substr(0, 2); //Detects char

        if (prefix == "v ") //If v is detected, enters function
        {
            struct Vec3 vertices;
            sscanf(line.c_str(), "v %f %f %f", &vertices.x, &vertices.y, &vertices.z); //Reads and stores the data into float variables
            vertexs[a] = vertices; //Stores the structure in an array
            a++;
            if (a >= ver)
            {
                vertexs = (Vec3*)realloc(vertexs, 2 * ver * sizeof(Vec3));
                ver *= 2;   //Doubles the global variable for re-allocation for the dynamic array
            }
        }
        else if (prefix == "vt") //If vt is detected, enters function
        {
            hasVT = true;
            struct Vec2 vectex;
            sscanf(line.c_str(), "vt %f %f", &vectex.x, &vectex.y);
            textureV[b] = vectex;
            b++;
            if (b >= verT) //Increase size if needed
            {
                textureV = (Vec2*)realloc(textureV, 2 * verT * sizeof(Vec2));
                verT *= 2;
            }
        }
        else if (prefix == "vn") //If vn is detected, enters function
        {
            struct Vec3 vecnorm;
            sscanf(line.c_str(), "vn %f %f %f", &vecnorm.x, &vecnorm.y, &vecnorm.z);
            normals[c] = vecnorm;
            c++;
            if (c >= verN)
            {
                normals = (Vec3*)realloc(normals, 2 * verN * sizeof(Vec3));
                verN *= 2;
            }
        }
        else if (prefix == "f ") //If f is detected, enters function
        {
            faceAmount++;
            int counting = count(line.begin(), line.end(), '/'); //Counts the number of slashes in the face lists (6 - Triangle, 8 - Quad)
            struct Vec4 faceIn;
            if (counting == 6 && hasVT == true)
            {
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &faceIn.ver1, &faceIn.vext1, &faceIn.vnorm1, &faceIn.ver2, &faceIn.vext2, &faceIn.vnorm2, &faceIn.ver3, &faceIn.vext3, &faceIn.vnorm3); //Scans and read through the values to store
                faceIn.count = 3;
                faces[f] = faceIn;
                f++;
                if (f >= face)
                {
                    faces = (Vec4*)realloc(faces, 2 * face * sizeof(Vec4));
                    face *= 2;
                }
            }
            else if (counting == 6 && hasVT == false)
            {
                sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &faceIn.ver1, &faceIn.vnorm1, &faceIn.ver2, &faceIn.vnorm2, &faceIn.ver3, &faceIn.vnorm3); //Scans and read through the values to store
                faceIn.count = 3; // Number of faces in the current list is 3
                faces[f] = faceIn;
                f++;
                if (f >= face) //Increases the size if needed
                {
                    faces = (Vec4*)realloc(faces, 2 * face * sizeof(Vec4));
                    face *= 2;
                }
            }
            else if (counting == 8 && hasVT == true)
            {
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &faceIn.ver1, &faceIn.vext1, &faceIn.vnorm1, &faceIn.ver2, &faceIn.vext2, &faceIn.vnorm2, &faceIn.ver3, &faceIn.vext3, &faceIn.vnorm3, &faceIn.ver4, &faceIn.vext4, &faceIn.vnorm4); //Scans and read through the values to store
                faceIn.count = 4; // Number of faces in the current list is 4
                faces[f] = faceIn;
                f++;
                if (f >= face)
                {
                    faces = (Vec4*)realloc(faces, 2 * face * sizeof(Vec4));
                    face *= 2;
                }
            }
            else if (counting == 8 && hasVT == false)
            {
                sscanf(line.c_str(), "f %d//%d %d//%d %d//%d %d//%d", &faceIn.ver1, &faceIn.vnorm1, &faceIn.ver2, &faceIn.vnorm2, &faceIn.ver3, &faceIn.vnorm3, &faceIn.ver4, &faceIn.vnorm4); //Scans and read through the values to store
                faceIn.count = 4;
                faces[f] = faceIn;
                f++;
                if (f >= face)
                {
                    faces = (Vec4*)realloc(faces, 2 * face * sizeof(Vec4));
                    face *= 2;
                }
            }

        }
    }

    int num;
    num = glGenLists(1);
    glNewList(num, GL_COMPILE);
    for (int i = 0; i < faceAmount; i++)
    {
        if (faces[i].count == 4)  //For Quads
        {
            glBegin(GL_QUADS);
            glNormal3f(normals[faces[i].vnorm1].x, normals[faces[i].vnorm1].y, normals[faces[i].vnorm1].z);
            if(hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext1].x, textureV[faces[i].vext1].y);
            }
            glVertex3f(vertexs[faces[i].ver1].x, vertexs[faces[i].ver1].y, vertexs[faces[i].ver1].z);

            glNormal3f(normals[faces[i].vnorm2].x, normals[faces[i].vnorm2].y, normals[faces[i].vnorm2].z);
            if(hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext2].x, textureV[faces[i].vext2].y);
            }
            glVertex3f(vertexs[faces[i].ver2].x, vertexs[faces[i].ver2].y, vertexs[faces[i].ver2].z);

            glNormal3f(normals[faces[i].vnorm3].x, normals[faces[i].vnorm3].y, normals[faces[i].vnorm3].z);
            if(hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext3].x, textureV[faces[i].vext3].y);
            }
            glVertex3f(vertexs[faces[i].ver3].x, vertexs[faces[i].ver3].y, vertexs[faces[i].ver3].z);

            glNormal3f(normals[faces[i].vnorm4].x, normals[faces[i].vnorm4].y, normals[faces[i].vnorm4].z);
            if(hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext4].x, textureV[faces[i].vext4].y);
            }
            glVertex3f(vertexs[faces[i].ver4].x, vertexs[faces[i].ver4].y, vertexs[faces[i].ver4].z);

            glEnd();
        }
        else if (faces[i].count == 3) //For Triangles
        {

            glBegin(GL_TRIANGLES);

            glNormal3f(normals[faces[i].vnorm1].x, normals[faces[i].vnorm1].y, normals[faces[i].vnorm1].z);
            if (hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext1].x, textureV[faces[i].vext1].y);
            }
            glVertex3f(vertexs[faces[i].ver1].x, vertexs[faces[i].ver1].y, vertexs[faces[i].ver1].z);

            glNormal3f(normals[faces[i].vnorm2].x, normals[faces[i].vnorm2].y, normals[faces[i].vnorm2].z);
            if (hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext2].x, textureV[faces[i].vext2].y);
            }
            glVertex3f(vertexs[faces[i].ver2].x, vertexs[faces[i].ver2].y, vertexs[faces[i].ver2].z);

            glNormal3f(normals[faces[i].vnorm3].x, normals[faces[i].vnorm3].y, normals[faces[i].vnorm3].z);
            if (hasVT == true)
            {
                glTexCoord2f(textureV[faces[i].vext3].x, textureV[faces[i].vext3].y);
            }
            glVertex3f(vertexs[faces[i].ver3].x, vertexs[faces[i].ver3].y, vertexs[faces[i].ver3].z);

            glEnd();
        }

    }
    glEndList(); //End of list
    return num; //Returns list ID
}

//-- Texture Loader Function --
void TextureLoader(void)
{
    glEnable(GL_TEXTURE_2D); // Enables 2D-Texturing
    glGenTextures(6, tex); //To generate the textures for each side of skybox
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    int width, height; //For the image reader
    unsigned char* image;

    //-- SKYBOX FRONT --
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    image = SOIL_load_image("image/sides.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image); // Binding the image
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // To magnify and minify the operations
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //-- SKYBOX BACK --
    glBindTexture(GL_TEXTURE_2D, tex[1]);
    image = SOIL_load_image("image/sides.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //-- SKYBOX TOP --
    glBindTexture(GL_TEXTURE_2D, tex[2]);
    image = SOIL_load_image("image/top.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // -- SKYBOX BOTTOM --
    glBindTexture(GL_TEXTURE_2D, tex[3]);
    image = SOIL_load_image("image/ground.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // -- SKYBOX RIGHT --
    glBindTexture(GL_TEXTURE_2D, tex[4]);
    image = SOIL_load_image("image/sides.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // -- SKYBOX LEFT --
    glBindTexture(GL_TEXTURE_2D, tex[5]);
    image = SOIL_load_image("image/sides.jpg", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

//-- Mouse Movement Function --
void mouseMove(int x, int y) //The mouse movement
{
    static float prev_x = 0.0;
    static float prev_y = 0.0;

    prev_x = (float)x - prev_x;
    prev_y = (float)y - prev_y;

    if ((abs((int)prev_x) > 15) || (abs((int)prev_y) > 15))
    {
        prev_x = (float)x;
        prev_y = (float)y;
        return;
    }

    thetaX = (int)(thetaX + prev_y) % 360;
    thetaY = (int)(thetaY + prev_x) % 360;

}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
    case 'q':
        exit(0);
        break;

    case 'o': //Resets the skybox
        moveLeftRight = 0;
        moveUpDown = 0;
        break;

    case 'r': // Press 'r' to enable or disable the wireframe
        WireFrame = !WireFrame;
        break;

    case 32:
        spaceBar = !spaceBar; //Resets everything
        break;

    case 'w': //Move forward
        movement += 1.0;
        break;

    case 's': //Move backwards
        movement -= 1.0;
        break;

    case 'a': //Move left
        moveLeftRight += 1.0;
        break;

    case 'd': //Move right
        moveLeftRight -= 1.0;
        break;
    }
}
void Specialkeys(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP: //Rotate upwards
        rotateY += 5;
        break;
    case GLUT_KEY_DOWN: //Rotate downwards
        rotateY -= 5;
        break;
    case GLUT_KEY_LEFT: //Rotate towards the left
        rotateX -= 5;
        break;
    case GLUT_KEY_RIGHT: //Rotate towards the right
        rotateX += 5;
        break;

    }
    glutPostRedisplay();
}


//-- Display Function --
static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(moveLeftRight, moveUpDown, movement); //Movement of camera using WASD and for zooming in and out
    glRotatef(thetaX, 20, 0, 0);//Mouse movement x-axis
    glRotatef(thetaY, 0, 20, 0); //Mouse movement y-axis
    glRotatef(rotateX, 0, 1, 0); //Rotating skybox
    glRotatef(rotateY, 1, 0, 0); //Rotating skybox

    gluLookAt(0, 0, 30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 100);

    if (WireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);		//Draw Our Mesh In Wireframe Mesh
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		//Toggle WIRE FRAME

    if (spaceBar == true) //This resets the entire scene when pressed
    {
        movement = 0;
        moveLeftRight = 0;
        moveUpDown = 0;
        rotateX = 0;
        rotateY = 0;
        spaceBar = false;
        thetaX = 0;
        thetaY = 0;
    }

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, tex[0]);
    glBegin(GL_QUADS); //Texture and Quad Points for Front
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -20.0f, 30.0f);
    glTexCoord2f(-1.0f, 0.0f); glVertex3f(30.0f, -20.0f, 30.0f);
    glTexCoord2f(-1.0f, -1.0f); glVertex3f(30.0f, 20.0f, 30.0f);
    glTexCoord2f(0.0f, -1.0f); glVertex3f(-30.0f, 20.0f, 30.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[1]);
    glBegin(GL_QUADS); //Texture and Quad Points for Back
    glTexCoord2f(-1.0f, 0.0f); glVertex3f(-30.0f, -20.0f, -30.0f);
    glTexCoord2f(-1.0f, -1.0f); glVertex3f(-30.0f, 20.0f, -30.0f);
    glTexCoord2f(0.0f, -1.0f); glVertex3f(30.0f, 20.0f, -30.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, -20.0f, -30.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[2]);
    glBegin(GL_QUADS); //Texture and Quad Points for Top
    glTexCoord2f(0.0f, -1.0f); glVertex3f(-30.0f, 20.0f, -30.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, 20.0f, 30.0f);
    glTexCoord2f(-1.0f, 0.0f); glVertex3f(30.0f, 20.0f, 30.0f);
    glTexCoord2f(-1.0f, -1.0f); glVertex3f(30.0f, 20.0f, -30.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[3]);
    glBegin(GL_QUADS); //Texture and Quad Points for Bottom
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-30.0f, -20.0f, -30.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(30.0f, -20.0f, -30.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, -20.0f, 30.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-30.0f, -20.0f, 30.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[4]);
    glBegin(GL_QUADS); //Texture and Quad Points for Right
    glTexCoord2f(-1.0f, 0.0f); glVertex3f(30.0f, -20.0f, -30.0f);
    glTexCoord2f(-1.0f, -1.0f); glVertex3f(30.0f, 20.0f, -30.0f);
    glTexCoord2f(0.0f, -1.0f); glVertex3f(30.0f, 20.0f, 30.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(30.0f, -20.0f, 30.0f);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, tex[5]);
    glBegin(GL_QUADS); //Texture and Quad Points for Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-30.0f, -20.0f, -30.0f);
    glTexCoord2f(-1.0f, 0.0f); glVertex3f(-30.0f, -20.0f, 30.0f);
    glTexCoord2f(-1.0f, -1.0f); glVertex3f(-30.0f, 20.0f, 30.0f);
    glTexCoord2f(0.0f, -1.0f); glVertex3f(-30.0f, 20.0f, -30.0f);
    glEnd();
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    //-- Cow Model --
    glPushMatrix();
    glTranslatef(10.0,0.0,10.0);
    glScalef(0.75,0.75,0.75);
    glCallList(cow); //Calls cow model
    glPopMatrix();

    //-- Elephant Model --
    glPushMatrix();
    glScalef(0.01,0.01,0.01);
    glTranslatef(-10.0,0.0,15.0);
    glCallList(ele); //Calls ele model
    glPopMatrix();

    //-- Trunk Model --
    glPushMatrix();
    glTranslatef(12.0,-20.0,-15.0);
    glScalef(2.5, 2.5, 2.5);
    glCallList(trunk); //Calls trunk model
    glPopMatrix();

    //-- Ven Model --
    glPushMatrix();
    glTranslatef(-10.0,-10.0,-20.0);
    glScalef(0.005,0.005,0.005);
    glCallList(ven); //Calls ven model
    glPopMatrix();

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
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

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    cow =  loadObject("model/cow.obj"); //Loading cow model
    ele = loadObject("model/ele.obj"); //Loading ele model
    trunk = loadObject("model/trunk.obj"); //Loading trunk model
    ven = loadObject("model/ven-2.obj"); //Loading ven model

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    TextureLoader(); //Calling texture loader function
    free(vertexs);  // De-allocating the Dynamic Arrays
    free(normals);
    free(textureV);

}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Project #5 (Model Loader)");
    init();

    glutMotionFunc(mouseMove);
    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutSpecialFunc(Specialkeys);

    glutIdleFunc(idle);
    glutMainLoop();

    return EXIT_SUCCESS;
}
