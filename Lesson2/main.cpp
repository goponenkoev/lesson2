//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <GL/GLAux.h>
#include <iostream>
#include <windows.h>       // Заголовочный файл для Windows
#include <math.h>          // Заголовочный файл для математической библиотеки

using namespace std;
GLsizei Height, Width;
unsigned int photo_tex;
AUX_RGBImageRec* photo_image;

unsigned int torus_tex;
AUX_RGBImageRec* torus_image;

GLfloat light_x = 0;  
GLfloat light0_color[] = {0.0, 0.0, 0.0, 1.0};
GLfloat blankMaterial[] = {0.6, 0.6, 0.6};
bool light0 = false, light1 = false;

void changeViewPort(int width, int height)
{
	glViewport(0,0,width,height);
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho(-5,5, -5,5, 2,12);   
	gluLookAt( 0,0,5, 0,0,0, 0,1,0 );
	glMatrixMode( GL_MODELVIEW );
}

void processSpecialKeys(int key, int x, int y) {
	switch(key) {
	case GLUT_KEY_LEFT:
		light0_color[0] = 1.0; light0_color[1] = 1.0; light0_color[2] = 0.0; break;
	case GLUT_KEY_RIGHT:
		light0_color[0] = 0.0; light0_color[1] = 1.0; light0_color[2] = 1.0; break;
	case GLUT_KEY_F3:
		light0_color[0] = 0.0; light0_color[1] = 0.0; light0_color[2] = 0.0; break;
	case GLUT_KEY_UP:
		blankMaterial[0] = 0.0; blankMaterial[1] = 0.0; blankMaterial[2] = 0.0;
		break;
	case GLUT_KEY_DOWN:
		blankMaterial[0] = 1.0; blankMaterial[1] = 1.0; blankMaterial[2] = 1.0;
		break;
	case GLUT_KEY_F1:
		if(light0){
			light0 = false;
			glEnable(GL_LIGHT0);
		}
		else{
			light0 = true;
			glDisable(GL_LIGHT0);
		}
		break;
	case GLUT_KEY_F2:
		if(light1){
			light1 = false;
			glEnable(GL_LIGHT1);
		}
		else{
			light1 = true;
			glDisable(GL_LIGHT1);
		}
		break;
	}
}

GLvoid Draw(GLvoid)									
{
	float specular[4] = {1, 1, 1, 1};
	float diffuse[4] = {1, 1, 0.2, 1}; 
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.0, 1.0, 1.0);
	GLfloat light0_position[] = {light_x, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, blankMaterial); 
	glPopMatrix();

	GLfloat tmp[] = {0,0,0,0};
	glPushMatrix();
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, photo_tex);
	glTranslatef(1,0,0);
	glColor3d(1,1,1);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,tmp);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tmp);
	glutSolidSphere(1,10,10);
	glDisable(GL_TEXTURE_2D);
	glColor4d(0,1,1, 0.5);
	auxSolidCylinder(2,3);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3,-1,0);
	glRotatef(-90, 1,0,0);
	glColor4d(1,1,1, 1);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, blankMaterial);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blankMaterial);
	glutSolidCone(1,3,10,10);
	glPopMatrix();

	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);

	//if (light_x < 6) light_x++; 
	if (light_x > -6) light_x--; else light_x = 0;
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Draw();
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {
	float pos[4] = {0,0,0,0};
	//float dir[3] = {-1,-1,-1};
	GLfloat mat_specular[] = { 1,1,1,1};

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Hello, GL");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glEnable(GL_AUTO_NORMAL);



	photo_image = auxDIBImageLoad("E://1.bmp");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir);
	glGenTextures(1, &photo_tex);
	glBindTexture(GL_TEXTURE_2D, photo_tex );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 
		photo_image->sizeX,
		photo_image->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE,
		photo_image->data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLfloat light1_diffuse[] = {0.4, 0.7, 0.2};
	GLfloat light1_direction[] = {5.0, 0.0,0.0, 0.0};
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_direction);

	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(render);

	GLenum err = glewInit();

	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	glutSpecialFunc(processSpecialKeys);
	glutMainLoop();
	return 0;
}