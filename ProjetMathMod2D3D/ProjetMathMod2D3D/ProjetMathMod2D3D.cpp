// ProjetMathMod2D3D.cpp : définit le point d'entrée pour l'application console.
//
// Specifique a Windows
#if _WIN32
#include <Windows.h>
#define FREEGLUT_LIB_PRAGMAS 0
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glu32.lib")
#endif
#if !defined(GLUT_WHEEL_UP)
# define GLUT_WHEEL_UP 3
# define GLUT_WHEEL_DOWN 4
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
void menu(int i);
void extrudeMenu(int i);
void myDisplay();
void myReshape(int w, int h);
void myMouse(int, int, int, int);
void myMouseMotion(int, int);
void SpecialInput(int key, int x, int y);
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
Point* controledPoint;
Point rotation = Point(0, 0, 0);
Point mousePosition;
int mouseButton;
int zoom = 500;
bool modeBSpline = false;
bool modeExtSimple = false;
bool modeExtRevolving = false;
bool modeExtGeneralize = false;
bool modeExtTriangle = false;
bool modeBezier = false;
int countUp = -1;
bool changeAxis = false;
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	init();
	glutMainLoop();

	return 0;
}

void init()
{
	//glShadeModel(GL_SMOOTH);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(10, 10);
	num = glutCreateWindow("Curves");
	glutMouseFunc(myMouse);
	glutMotionFunc(myMouseMotion);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(SpecialInput);
	createMenu();
}
void myMouse(int button, int state, int x, int y)
{
	mouseButton = button;
	Point p = convert(Point(static_cast<float>(x), static_cast<float>(y), 0.f));
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		for (auto it = bSplines->begin(); it < bSplines->end(); ++it)
		{
			if (!(*it)->isControlPoint(p, controledPoint))
			{
				if (modeBSpline)
				{
					(*bSplines)[bSplines->size() - 1]->addControlPoint(Point(p.getX(), p.getY(), p.getZ()));

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
		}

	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		std::cout << "clic (" << p.getX() << ", " << p.getY() << ")" << std::endl;
		mousePosition.setX(p.getX());
		mousePosition.setY(p.getY());
	}
	else if (state == GLUT_UP)
	{
		if (button == GLUT_WHEEL_UP)
		{
			std::cout << "Wheel Up" << std::endl;
			zoom -= 10;
		}
		else if (button == GLUT_WHEEL_DOWN)
		{
			std::cout << "Wheel Down" << std::endl;
			zoom += 10;
		}
	}

	glutPostWindowRedisplay(num);
}
void myMouseMotion(int x, int y)
{
	Point p = convert(Point(static_cast<float>(x), static_cast<float>(y), 0.f));
	switch (mouseButton)
	{
	case GLUT_MIDDLE_BUTTON: //clic droit enfoncé
		std::cout << "middle move" << std::endl;
		rotation.setY(rotation.getY() + (p.getX() - mousePosition.getX())*0.1);
		rotation.setX(rotation.getX() + (p.getY() - mousePosition.getY())*0.1);
		mousePosition.setX(p.getX());
		mousePosition.setY(p.getY());
		break;
	case GLUT_LEFT_BUTTON:
		if (controledPoint != nullptr)
		{
			controledPoint->setX(p.getX());
			controledPoint->setY(p.getY());
			(*bSplines)[bSplines->size() - 1]->generateVecteurNodal();
			(*bSplines)[bSplines->size() - 1]->generateBSplineCurve();

			(*bSplines)[bSplines->size() - 1]->setClosedBSpline(false);
			(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
			(*bSplines)[bSplines->size() - 1]->setDegree(3);
			(*bSplines)[bSplines->size() - 1]->closeBSpline();
		}

		break;
	}
	glutPostWindowRedisplay(num);
}
void SpecialInput(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		controledPoint->setY(controledPoint->getY() + 5);
		(*bSplines)[bSplines->size() - 1]->generateVecteurNodal();
		(*bSplines)[bSplines->size() - 1]->generateBSplineCurve();

		(*bSplines)[bSplines->size() - 1]->setClosedBSpline(false);
		(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
		(*bSplines)[bSplines->size() - 1]->setDegree(3);
		(*bSplines)[bSplines->size() - 1]->closeBSpline();
		break;
	case GLUT_KEY_DOWN:
		controledPoint->setY(controledPoint->getY() - 5);
		(*bSplines)[bSplines->size() - 1]->generateVecteurNodal();
		(*bSplines)[bSplines->size() - 1]->generateBSplineCurve();

		(*bSplines)[bSplines->size() - 1]->setClosedBSpline(false);
		(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
		(*bSplines)[bSplines->size() - 1]->setDegree(3);
		(*bSplines)[bSplines->size() - 1]->closeBSpline();
		break;
	case GLUT_KEY_LEFT:
		if (controledPoint != nullptr)
		{
			if (changeAxis)
			{
				controledPoint->setZ(controledPoint->getZ() - 5);
			}
			else
			{
				controledPoint->setX(controledPoint->getX() - 5);
			}
			(*bSplines)[bSplines->size() - 1]->generateVecteurNodal();
			(*bSplines)[bSplines->size() - 1]->generateBSplineCurve();

			(*bSplines)[bSplines->size() - 1]->setClosedBSpline(false);
			(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
			(*bSplines)[bSplines->size() - 1]->setDegree(3);
			(*bSplines)[bSplines->size() - 1]->closeBSpline();
		}
		break;
	case GLUT_KEY_RIGHT:
		if (controledPoint != nullptr)
		{
			if (changeAxis)
			{
				controledPoint->setZ(controledPoint->getZ() + 5);
			}
			else
			{
				controledPoint->setX(controledPoint->getX() + 5);
			}
			(*bSplines)[bSplines->size() - 1]->generateVecteurNodal();
			(*bSplines)[bSplines->size() - 1]->generateBSplineCurve();

			(*bSplines)[bSplines->size() - 1]->setClosedBSpline(false);
			(*bSplines)[bSplines->size() - 1]->setAccuracy(100.f);
			(*bSplines)[bSplines->size() - 1]->setDegree(3);
			(*bSplines)[bSplines->size() - 1]->closeBSpline();
		}
		break;
	}
	glutPostWindowRedisplay(num);
}
void myKeyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':
		zoom += 10;
		break;
	case '1':
		if ((*bSplines)[bSplines->size() - 1]->getControlPoint().size() > 0)
		{
			countUp = countUp == (*bSplines)[bSplines->size() - 1]->getControlPoint().size() - 1 ? 0 : countUp + 1;
			controledPoint = &(*bSplines)[bSplines->size() - 1]->getControlPoint()[countUp];
		}
		break;
	case 'a':
		rotation.setY(rotation.getY() + 90);
		changeAxis = true;
		break;
	case 'z':
		rotation.setX(rotation.getX() + 90);
		break;
	case 'q':
		rotation.setX(0);
		rotation.setY(0);
		changeAxis = false;

		break;
	default:
		break;
	}
	glutPostWindowRedisplay(num);
}
void myDisplay()
{
	glColor3f(0.4f, 0.4f, 0.4f);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(0, 0, zoom, 0, 0, 0, 0, 1, 0);
	// Rotation de la Caméra
	glRotatef(rotation.getX(), 1, 0, 0);
	glRotatef(rotation.getY(), 0, 1, 0);
	glRotatef(rotation.getZ(), 0, 0, 1);

	for (auto it = bSplines->begin(); it != bSplines->end(); ++it)
	{
		(*it)->drawControlPoints(15);
		glColor3f(0.0, 0.4, 1.0);//blue
		(*it)->drawBSplineCurve();
		glColor3f(0.4, 0.4, 0.4);//grey
	}
	if (countUp > -1)
	{
		glColor3f(1.0, 0.1, 0.0);
		(*bSplines)[bSplines->size() - 1]->drawControlPoint(countUp);
		glColor3f(0.4, 0.4, 0.4);
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
	gluPerspective(70., (GLfloat)w / h, 1, 1000);
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
	ret.setZ(p.getZ());
	return ret;
}

