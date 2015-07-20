#pragma once

#include <vector>

#include "Point.h"

class BezierSurface
{
	private :
		float _accuracy;							// pr�cision
		int _m;										// nb Point de controle par ligne
		int _n;										// nb Point de controle par colonne
		std::vector<Point> _surfaceControlPoints;	// Contient les points de controles (nuage de point)
		std::vector<Point> _surfaceBezierPoints;	// Contient les points de la surface � Bezier g�n�rer � partir des points de controles

	public:
		BezierSurface();
		~BezierSurface();

		// G�n�re les points de controles (nuage de points automatiquement de facon d�t�rmin�)
		void generateSurfaceControlPoints();

		// Fonction appliquant l'algorithme deCaseljou
		Point deCaseljou(std::vector<Point> curve, float step);
		// FOnction g�n�rant la surface de Bezier en utiliser les points de controle et la fonction deCaseljou
		void generateBezierSurface();

		// G�n�ration de la liste contenant l'indice des faces triangulaires � g�n�rer
		std::vector<int> generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int width, const unsigned int height);

};

