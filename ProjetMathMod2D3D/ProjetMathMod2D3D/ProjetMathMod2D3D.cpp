// ProjetMathMod2D3D.cpp : définit le point d'entrée pour l'application console.
//
// Specifique a Windows
#if _WIN32
#include <Windows.h>
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#endif

// Entete OpenGL 
#define GLEW_STATIC 1
#include <include/GL/glew.h>
#include <GL/glut.h>
#include <gl/Gl.h>
#include <gl/Glu.h>

#include "Include/Point.h"
#include "Include\util.hpp"
#include "Include/BSpline.h"
#include "Include/Extrude.h"

//------------------------//
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
int num = 0;

void init();
void myMenu(int i);
void myDisplay();
void myReshape(int w, int h);
void myMouse(int, int, int, int);
void myMouseMotion(int, int);
void myKeyboard(unsigned char key, int x, int y);
void createMenu();
Point convert(Point p);


std::vector<BSpline*> *bSplines = new std::vector<BSpline*>();

int main(int argc, char** argv)
{
	/*
	std::cout << std::endl;
	std::cout << "--------- BEGIN ----------" << std::endl;
	std::cout << std::endl;

	BSpline b;

	b.addControlPoint(Point(-1.0f, 0.0f, 0.0f));
	b.addControlPoint(Point(2.0f, 5.0f, 0.0f));
	b.addControlPoint(Point(4.0f, 5.0f, 0.0f));
	b.addControlPoint(Point(6.0f, 0.0f, 0.0f));

	b.generateVecteurNodal();

	for (unsigned int i = 0; i < b.getNodalVector().size(); ++i)
	{
		std::cout << b.getNodalVector()[i] << " | " << b.getNormalizedNodalVector()[i] << std::endl;
	}

	std::cout << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << std::endl;

	b.generateBSplineCurve();

	b.setClosedBSpline(true);
	//b.setClosedBSpline(false);
	b.closeBSpline();

	for (unsigned int i = 0; i < b.getBSplineCurve().size(); ++i)
	{
		std::cout << b.getBSplineCurve()[i] << std::endl;
	}

	std::cout << std::endl;
	std::cout << "--------------------------" << std::endl;
	std::cout << std::endl;

	Extrude ext = Extrude();

	for (unsigned int i = 0; i < ext.getGeneralizedExtrudeBSpline().getBSplineCurve().size(); ++i)
		std::cout << ext.getGeneralizedExtrudeBSpline().getBSplineCurve()[i] << std::endl;


	std::cout << std::endl;
	std::cout << "--------- END ----------" << std::endl;
	std::cout << std::endl;
	*/

	//std::cin.ignore();

	glutInit(&argc, argv);
	init();
	glutMainLoop();




	return 0;
}

void init()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(10, 10);
	num = glutCreateWindow("Bezier Curves");
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseMotion);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(myKeyboard);
	createMenu();
}
bool toto = false;
void myMouse(int button, int state, int x, int y)
{
	Point p = convert(Point(x, y, 0));
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "ajout d'un point de controle en : (" << p.getX() << ", " << p.getY() << ", " << p.getZ() << ") " << std::endl;
		(*bSplines)[0]->addControlPoint(Point(p.getX(), p.getY(), 0));

		if ((*bSplines)[0]->getControlPoint().size() >= 3)
		{
			(*bSplines)[0]->generateVecteurNodal();
			(*bSplines)[0]->generateBSplineCurve();

			(*bSplines)[0]->setClosedBSpline(false);
			//b.setClosedBSpline(false);
			(*bSplines)[0]->closeBSpline();

		}
	}

	glutPostWindowRedisplay(num);
}
void myMouseMotion(int x, int y)
{
	glutPostWindowRedisplay(num);
}
void myKeyboard(unsigned char key, int x, int y)
{
	glutPostWindowRedisplay(num);
}
void myDisplay()
{
	glColor3f(0.4, 0.4, 0.4);
	glClear(GL_COLOR_BUFFER_BIT);
	for (auto it = bSplines->begin(); it < bSplines->end(); ++it)
	{
		(*it)->drawControlPoints(10);
		(*it)->drawBSplineCurve();
	}
	glutSwapBuffers();
}
void myReshape(int w, int h)
{
	glutInitWindowSize(w, h);
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}
void myMenu(int i)
{
	switch (i)
	{
	case 0:
	{
		std::cout << "create BSpline :" << std::endl;
		bSplines->push_back(new BSpline());
		break;
	}
	case 1:
		std::cout << "End of curve :" << std::endl;
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 20:
		break;
	default:
		break;
	}
}
void createMenu()
{
	glutCreateMenu(myMenu);
	glutAddMenuEntry("Create BSpline", 0);
	glutAddMenuEntry("End of Curve", 1);
	glutAddMenuEntry("Move curve", 2);
	glutAddMenuEntry("Rescale curve", 3);
	glutAddMenuEntry("Rotate curve", 4);
	glutAddMenuEntry("C0", 5);

	glutAddMenuEntry("reset", 20);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}
Point convert(Point p)
{
	Point ret;
	ret.setX((2 * p.getX() - SCREEN_WIDTH) / 2);
	ret.setY(-(2 * p.getY() - SCREEN_HEIGHT) / 2);
	return ret;
}