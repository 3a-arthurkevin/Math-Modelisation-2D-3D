#pragma once

#include <vector>

#include "Point.h"

class BezierSurface
{
	private :
		float _accuracy;							// précision
		int _m;										// nb Point de controle par ligne
		int _n;										// nb Point de controle par colonne
		std::vector<Point> _surfaceControlPoints;	// Contient les points de controles (nuage de point)
		std::vector<Point> _surfaceBezierPoints;	// Contient les points de la surface à Bezier générer à partir des points de controles

	public:
		BezierSurface();
		~BezierSurface();

		// Génère les points de controles (nuage de points automatiquement de facon détérminé)
		void generateSurfaceControlPoints();

		// Fonction appliquant l'algorithme deCaseljou
		Point deCaseljou(std::vector<Point> curve, float step);
		// FOnction générant la surface de Bezier en utiliser les points de controle et la fonction deCaseljou
		void generateBezierSurface();

		// Génération de la liste contenant l'indice des faces triangulaires à générer
		std::vector<int> generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int width, const unsigned int height);

};

