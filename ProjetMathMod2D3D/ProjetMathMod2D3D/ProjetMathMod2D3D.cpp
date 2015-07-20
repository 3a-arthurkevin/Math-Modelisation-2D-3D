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
#include "Include\BezierSurface.h"
#include <memory>

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
	std::shared_ptr<Extrude> ext = nullptr;
	std::vector<Point> extrudeType;
	std::vector<int> triangle;
};

std::shared_ptr<BSpline> bSplines = nullptr;
std::shared_ptr<sExtrude> extrudes = nullptr;
std::shared_ptr<BezierSurface> bezierSur = nullptr;
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
	num = glutCreateWindow("Modélisation 2D - 3D");
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
	//std::cout << x << " - " << y << std::endl;
	mouseButton = button;
	Point p = convert(Point(static_cast<float>(x), static_cast<float>(y), 0.f));
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if(bSplines != nullptr)
		{
			if (!bSplines->isControlPoint(p, controledPoint))
			{
				if (modeBSpline)
				{
					bSplines->addControlPoint(Point(p.getX(), p.getY(), p.getZ()));

					if (bSplines->getControlPoint().size() >= 3)
					{
						bSplines->generateVecteurNodal();
						bSplines->generateBSplineCurve();

						bSplines->setClosedBSpline(false);
						bSplines->setAccuracy(100.f);
						bSplines->setDegree(3);
						bSplines->closeBSpline();
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
	case GLUT_MIDDLE_BUTTON: //clic middle enfoncé
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
			bSplines->generateVecteurNodal();
			bSplines->generateBSplineCurve();

			bSplines->setClosedBSpline(false);
			bSplines->setAccuracy(100.f);
			bSplines->setDegree(3);
			bSplines->closeBSpline();
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
		bSplines->generateVecteurNodal();
		bSplines->generateBSplineCurve();

		bSplines->setClosedBSpline(false);
		bSplines->setAccuracy(100.f);
		bSplines->setDegree(3);
		bSplines->closeBSpline();
		break;
	case GLUT_KEY_DOWN:
		controledPoint->setY(controledPoint->getY() - 5);
		bSplines->generateVecteurNodal();
		bSplines->generateBSplineCurve();

		bSplines->setClosedBSpline(false);
		bSplines->setAccuracy(100.f);
		bSplines->setDegree(3);
		bSplines->closeBSpline();
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
			bSplines->generateVecteurNodal();
			bSplines->generateBSplineCurve();

			bSplines->setClosedBSpline(false);
			bSplines->setAccuracy(100.f);
			bSplines->setDegree(3);
			bSplines->closeBSpline();
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
			bSplines->generateVecteurNodal();
			bSplines->generateBSplineCurve();

			bSplines->setClosedBSpline(false);
			bSplines->setAccuracy(100.f);
			bSplines->setDegree(3);
			bSplines->closeBSpline();
		}
		break;
	}
	if (modeExtSimple)
	{
		extrudeMenu(20);
	}
	else if (modeExtRevolving)
	{
		extrudeMenu(21);
	}
	else if (modeExtGeneralize)
	{
		extrudeMenu(22);
	}
	else if (modeExtTriangle)
	{
		extrudeMenu(23);
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
		if (bSplines->getControlPoint().size() > 0)
		{
			countUp = countUp == bSplines->getControlPoint().size() - 1 ? 0 : countUp + 1;
			controledPoint = &bSplines->getControlPoint()[countUp];
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

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE);
	glVertex3f(-10.f, 0.f, 0.f);
	glVertex3f(10.f, 0.f, 0.f);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE);
	glVertex3f(0.f, -10.f, 0.f);
	glVertex3f(0.f, 10.f, 0.f);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE);
	glVertex3f(0.f, 0.f, -10.f);
	glVertex3f(0.f, 0.f, 10.f);
	glEnd();

	glColor3f(0.4f, 0.4f, 0.4f);

	gluLookAt(0, 0, zoom, 0, 0, 0, 0, 1, 0);
	// Rotation de la Caméra
	glRotatef(rotation.getX(), 1, 0, 0);
	glRotatef(rotation.getY(), 0, 1, 0);
	glRotatef(rotation.getZ(), 0, 0, 1);

	if(bSplines != nullptr)
	{
		bSplines->drawControlPoints(15);
		glColor3f(0.0, 0.4, 1.0);//blue
		bSplines->drawBSplineCurve();
		glColor3f(0.4, 0.4, 0.4);//grey
	}
	if (countUp > -1)
	{
		glColor3f(1.0, 0.1, 0.0);
		bSplines->drawControlPoint(countUp);
		
	}

	if(extrudes != nullptr)
	{
		glColor3f(1.0, 1.0, 0.0);
		if (modeExtTriangle)
			extrudes->ext->drawCurve(extrudes->extrudeType);
		else
			extrudes->ext->drawCurve(extrudes->extrudeType, true);
	}
	if (bezierSur != nullptr)
	{
		bezierSur->drawControlPoints();
		bezierSur->drawSurface();
	}
	glColor3f(0.4, 0.4, 0.4);
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
		bSplines = std::shared_ptr<BSpline>(new BSpline());
		bSplines->setAccuracy(100.f);
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
		bezierSur = std::shared_ptr<BezierSurface>(new BezierSurface());
		bezierSur->generateBezierSurface();
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
		extrudes = std::shared_ptr<sExtrude>(new sExtrude());
		extrudes->ext = std::shared_ptr<Extrude>(new Extrude());
		extrudes->extrudeType = std::vector<Point>();
		extrudes->extrudeType = extrudes->ext->simpleExtrude(bSplines->getBSplineCurve(), true, 500);
		modeExtSimple = true;
		break;
	}
	case 21:
	{
		std::cout << "create Revolving Extrude :" << std::endl;
		extrudes = std::shared_ptr<sExtrude>(new sExtrude());
		extrudes->ext = std::shared_ptr<Extrude>(new Extrude());
		extrudes->extrudeType = std::vector<Point>();
		extrudes->extrudeType = extrudes->ext->revolvingExtrude(bSplines->getBSplineCurve(), false);
		modeExtRevolving = true;
		break;
	}
	case 22:
	{
		std::cout << "create Generalized Extrude :" << std::endl;
		extrudes = std::shared_ptr<sExtrude>(new sExtrude());
		extrudes->ext = std::shared_ptr<Extrude>(new Extrude());
		extrudes->extrudeType = std::vector<Point>();
		//se->extrudeType = se->ext->generalizedExtrude(bSplines->getBSplineCurve(), false, 10);
		modeExtGeneralize = true;
		break;
	}
	case 23:
	{
		std::cout << "create Triangular Faces :" << std::endl;
		extrudes = std::shared_ptr<sExtrude>(new sExtrude());
		extrudes->ext = std::shared_ptr<Extrude>(new Extrude());
		extrudes->extrudeType = std::vector<Point>();
		extrudes->triangle = extrudes->ext->generateTriangularFacesIndex(bSplines->getBSplineCurve(), 10, 10);
		modeExtTriangle = true;
		break;
	}
	default:
		break;
	}
	glutPostWindowRedisplay(num);
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
	glutAddMenuEntry("Bezier Surface", 4);
	//glutAddMenuEntry("C0", 5);

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

