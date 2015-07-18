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
#include "Include\camera.h"

//------------------------//
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
int num = 0;

void init();
void menu(int i);
void extrudeMenu(int i);
void myDisplay();
void myReshape(int w, int h);
void myMouse(int, int, int, int);
void myMouseMotion(int, int);
void myKeyboard(unsigned char key, int x, int y);
void createMenu();
Point convert(Point p);

struct  sExtrude
{
	Extrude *ext = nullptr;
	std::vector<Point> extrudeType;
	std::vector<int> triangle;
};

std::vector<BSpline*> *bSplines = new std::vector<BSpline*>();
std::vector<sExtrude*> *extrudes = new std::vector<sExtrude*>();
CCamera Camera;
bool modeBSpline = false;
bool modeExtSimple = false;
bool modeExtRevolving = false;
bool modeExtGeneralize = false;
bool modeExtTriangle = false;
bool modeBezier = false;

void DrawNet(GLfloat size, GLint LinesX, GLint LinesZ)
{
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(-size / 2.0 + xc / (GLfloat)(LinesX - 1)*size,
			0.0,
			size / 2.0);
		glVertex3f(-size / 2.0 + xc / (GLfloat)(LinesX - 1)*size,
			0.0,
			size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(size / 2.0,
			0.0,
			-size / 2.0 + zc / (GLfloat)(LinesZ - 1)*size);
		glVertex3f(size / -2.0,
			0.0,
			-size / 2.0 + zc / (GLfloat)(LinesZ - 1)*size);
	}
	glEnd();
}

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
void myMouse(int button, int state, int x, int y)
{
	Point p = convert(Point(static_cast<float>(x), static_cast<float>(y), 0.f));
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (modeBSpline)
		{
			(*bSplines)[bSplines->size() - 1]->addControlPoint(Point(p.getX(), p.getY(), 0));

			if ((*bSplines)[bSplines->size() - 1]->getControlPoint().size() >= 3)
			{
				(*bSplines)[bSplines->size() - 1]->generateVecteurNodal();
				(*bSplines)[bSplines->size() - 1]->generateBSplineCurve();

				(*bSplines)[bSplines->size() - 1]->setClosedBSpline(false);
				(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
				(*bSplines)[bSplines->size() - 1]->setDegree(3);
				(*bSplines)[bSplines->size() - 1]->closeBSpline();

			}
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
	switch (key)
	{
	case 27:		//ESC
		PostQuitMessage(0);
		break;
	case 'a':
		Camera.RotateY(5.0);
		break;
	case 'd':
		Camera.RotateY(-5.0);
		break;
	case 'w':
		Camera.MoveForwards(-0.1);
		break;
	case 's':
		Camera.MoveForwards(0.1);
		break;
	case 'x':
		Camera.RotateX(5.0);
		break;
	case 'y':
		Camera.RotateX(-5.0);
		break;
	case 'c':
		Camera.StrafeRight(-0.1);
		break;
	case 'v':
		Camera.StrafeRight(0.1);
		break;
	case 'f':
		Camera.Move(F3dVector(0.0, -0.3, 0.0));
		break;
	case 'r':
		Camera.Move(F3dVector(0.0, 0.3, 0.0));
		break;

	}
	glutPostWindowRedisplay(num);
}
void myDisplay()
{
	glColor3f(0.4f, 0.4f, 0.4f);
	glClear(GL_COLOR_BUFFER_BIT);
	Camera.Render();
	for (auto it = bSplines->begin(); it != bSplines->end(); ++it)
	{
		(*it)->drawControlPoints(10);
		(*it)->drawBSplineCurve();
	}
	for (auto it = extrudes->begin(); it != extrudes->end(); ++it)
	{
		if (modeExtTriangle)
			(*it)->ext->drawCurve((*it)->extrudeType);
		else
			(*it)->ext->drawCurve((*it)->extrudeType, true);
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
void menu(int i)
{
	switch (i)
	{
	case 0:
	{
		std::cout << "create BSpline :" << std::endl;
		bSplines->push_back(new BSpline());
		(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
		modeBSpline = true;
		break;
	}
	case 1:
		std::cout << "End of BSpline :" << std::endl;
		modeBSpline = false;
		break;
	case 3:
		std::cout << "End of Extrude :" << std::endl;
		modeExtSimple = false;
		modeExtGeneralize = false;
		modeExtRevolving = false;
		modeExtTriangle = false;
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
void extrudeMenu(int i)
{
	switch (i)
	{
	case 20:
	{
		std::cout << "create Simple Extrude :" << std::endl;
		sExtrude *se = new sExtrude();
		se->ext = new Extrude();
		se->extrudeType = std::vector<Point>();
		se->extrudeType = se->ext->simpleExtrude((*bSplines)[bSplines->size() - 1]->getBSplineCurve(), true, 0.5);
		extrudes->push_back(se);
		modeExtSimple = true;
		break;
	}
	case 21:
	{
		std::cout << "create Revolving Extrude :" << std::endl;
		sExtrude *se = new sExtrude();
		se->ext = new Extrude();
		se->extrudeType = std::vector<Point>();
		se->extrudeType = se->ext->revolvingExtrude((*bSplines)[bSplines->size() - 1]->getBSplineCurve(), false);
		extrudes->push_back(se);
		modeExtRevolving = true;
		break;
	}
	case 22:
	{
		std::cout << "create Generalized Extrude :" << std::endl;
		sExtrude *se = new sExtrude();
		se->ext = new Extrude();
		se->extrudeType = std::vector<Point>();
		//se->extrudeType = se->ext->generalizedExtrude((*bSplines)[bSplines->size() - 1]->getBSplineCurve(), false, 10);
		extrudes->push_back(se);
		modeExtGeneralize = true;
		break;
}
	case 23:
	{
		std::cout << "create Triangular Faces :" << std::endl;
		sExtrude *se = new sExtrude();
		se->ext = new Extrude();
		se->extrudeType = std::vector<Point>();
		se->triangle = se->ext->generateTriangularFacesIndex((*bSplines)[bSplines->size() - 1]->getBSplineCurve(), 10, 10);
		extrudes->push_back(se);
		modeExtTriangle = true;
		break;
			}
	default:
		break;
	}
}
void createMenu()
{
	int menuExtrude = glutCreateMenu(extrudeMenu);
	glutAddMenuEntry("Simple Extrude", 20);
	glutAddMenuEntry("Revolving Extrude", 21);
	glutAddMenuEntry("Generalized Extrude", 22);
	glutAddMenuEntry("Triangular Faces", 23);

	glutCreateMenu(menu);
	glutAddMenuEntry("Create BSpline", 0);
	glutAddMenuEntry("End of BSpline", 1);
	glutAddSubMenu("Create Extrude", menuExtrude);
	glutAddMenuEntry("End of Extrude", 3);
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

