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

#include "Include/Point.h"
#include "Include/BSpline.h"

int main(int argc, char** argv)
{
	std::cout << std::endl;

	std::cout << "--------- BEGIN ----------" << std::endl;

	std::cout << std::endl;

	BSpline b;

	b.addControlPoint(Point());
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

	for (unsigned int i = 0; i < b.getBSplineCurve().size(); ++i)
	{
		std::cout << b.getBSplineCurve()[i] << std::endl;
	}


	std::cout << std::endl;
	std::cout << "--------- END ----------" << std::endl;
	std::cout << std::endl;

	std::cin.ignore();

	return 0;
}

