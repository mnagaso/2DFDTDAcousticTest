/*
 * main.cpp
 *
 *  Created on: 2013/10/26
 *      Author: mnsaru
 */

using namespace std;

#include <stdio.h>
#include <string.h>
#include <iostream>
using std::ios;
using std::cin;
using std::cout;
using std::endl;

//#include <GL/gl.h>
#include <GLUT/glut.h>
//#include <GLUT/freeglut.h>
#include <math.h>
//#include <GLUI/GLUI.h>

#define x_fSize 200 //field size
#define y_fSize 200
#define t_step 200 //time steps
#define C0 1500// m/s, background sound speed (fastest speed in the region)
#define Sc 0.5 //Courant number
#define x_Sp 100 //sound source point x
#define y_Sp 100//source y
#define BlankSize 10//Blank window width
#define AMP 20//sound source Amplitude

double Vx[y_fSize][x_fSize];
double Vy[y_fSize][x_fSize];
double Pr[y_fSize][x_fSize];
double rho[y_fSize][x_fSize]; //density
double Cvxp[y_fSize][x_fSize];
double Cvyp[y_fSize][x_fSize];
double Cprv[y_fSize][x_fSize];
float Cr[y_fSize][x_fSize]; //relative sound speed
int globalTimeStep = 0;

/*prototype*/
void Draw2D();

void initRegion()
{
	cout << "/*field characteristics initialization*/" << endl;
	for(int i = 0; i < y_fSize; i++)
		for(int j = 0; j < x_fSize; j++)
		{
			rho[i][j] = 1.;
			Cr[i][j] = 1.;
			//if(j > 150)
				//Cr[i][j] = 0.7;
		}

	for(int i = 0; i < y_fSize; i++)
		for(int j = 0; j < x_fSize; j++)
		{
			Cvxp[i][j] = 2 * Sc / ((rho[i][j+1] + rho[i][j]) * C0);
			Cvyp[i][j] = 2 * Sc / ((rho[i+1][j] + rho[i][j]) * C0);
			Cprv[i][j] = rho[i][j] * Cr[i][j] * Cr[i][j] * C0 * Sc;
			Vx[i][j] = 0.;
			Vy[i][j] = 0.;
			Pr[i][j] = 0.;
		}
}
void calc()
{
	//cout << "calculation step of__" << globalTimeStep << endl;
	//for(int time = 0; time < t_step; time++)

	//cout << "time loop" << endl;
	//*Vx update equation*
	for(int i = 0; i < y_fSize - 1; i++)
		for(int j = 0; j < x_fSize - 1; j++)
			Vx[i][j] = Vx[i][j] - Cvxp[i][j] * ( Pr[i][j+1] - Pr[i][j]);
	//*Vy update equation*
	for(int i = 0; i < y_fSize - 1; i++)
		for(int j = 0; j < x_fSize - 1; j++)
			Vy[i][j] = Vy[i][j] - Cvyp[i][j] * ( Pr[i+1][j] - Pr[i][j]);
	//*Pr update equation*
	for(int i = 0; i < y_fSize; i++)
		for(int j = 0; j < x_fSize; j++)
			Pr[i][j] = Pr[i][j] - Cprv[i][j] * ( (Vx[i][j] - Vx[i][j-1])
												+(Vy[i][j] - Vy[i-1][j]) );
	//*additive source here*
	Pr[y_Sp][x_Sp] += AMP * exp(-(globalTimeStep - 30.) * (globalTimeStep - 30.) / 100.);

	//*set ABC boundary*
	for(int i = 0;i < y_fSize; i++)
	{
		//*upper edge of region*
		//Vx[i][0] = Vx[i][1];
		//Vy[i][0] = Vy[i][1];
		//Pr[i][0] = Pr[i][1];
		//*downer side of region*
		//Vx[i][0] = Vx[i][1];
		//Vy[i][0] = Vy[i][1];
		//Pr[i][0] = Pr[i][1];
		//*left side*
		//*right side*
	}

	cout << Vx[y_Sp][x_Sp] << "," << Vy[y_Sp][x_Sp] << "," << Pr[y_Sp][x_Sp] << endl;
}
static void DrawString(string str, int w, int h, int x0, int y0)
{
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
		gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glRasterPos2f(x0, y0);
	int size = (int)str.size();
	for(int i = 0; i < size; ++i){
		char ic = str[i];
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}
void Draw2D()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//cout << "Draw2D time step" << globalTimeStep << endl;

	//field condition draw
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3d(1.,0.,0.);
	glVertex2i(x_Sp, y_Sp);
	glEnd();
	//field situation draw
	glPointSize(1);
	glBegin(GL_POINTS);
	for(int i = 0; i < y_fSize; i++)
		for(int j = 0; j < x_fSize; j++)
		{
			float color = 0.5 + Pr[i][j];
			//cout << color << endl;
			//float color = 0.5;
			glColor3f(color, color, color);
			glVertex2i(j,i);
		}
	glEnd();

	//draw timestep indication
	char str[256];
	sprintf(str, "time step : %d", globalTimeStep);
	glColor3d(1.0,0.0,0.0);
	DrawString(str,glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT),12,12);

	glutSwapBuffers();

}
void display()
{
	Draw2D();
}
void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
}
void idle()
{
	//for(int t = 0; t < t_step; t++)
	//{
	calc();
	globalTimeStep += 1;

		//cout << "redrawing...time of..." << t << endl;
	glutPostRedisplay();
	//}
	//glutTimerFunc(50,timeloop,0);
}
void resize(int w, int h)
{
	//glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int fs = BlankSize;
	gluOrtho2D(0.0 - fs, x_fSize + fs, y_fSize + fs, 0.0 - fs);
}
int main(int argc, char **argv)
{
	initRegion();

	//initiation GLUT
	glutInit(&argc, argv);//initialize OpenGL environment
	glutInitWindowPosition(100,100);
	glutInitWindowSize(500,500);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	//glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow(argv[0]);


	//call back func
	glutDisplayFunc(display);
	//glutKeyboardFunc(keyboard);
	glutReshapeFunc(resize);
	init();
	glutIdleFunc(idle);
	//glutTimerFunc(100,timeloop,0);

	glutMainLoop();


	return 0;
}
