#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <algorithm>
#include <vector>

//#include "Includer.h"

#include "Point.h"
#include "BSpline.h"

class Extrude : Util
{
	private:
		float _accuracy;				// Précision lors de l'extrude
		bool _closedCurve;				// Fermeture de la courbe 
		float _coefficient;				// Coefficient pour l'extrude sur un axe
		BSpline _bSplineExtrude;		// B-Spline pour l'extrude généralisé

	public:
		Extrude();
		~Extrude();

		//Setter
		void setClosedCurve(bool value)
		{
			_closedCurve = value;
		}

		void setCoefficient(float value)
		{
			_coefficient = value;
		}

		void setGeneralizedExtrudeBSpline(BSpline bSpline)
		{
			_bSplineExtrude = bSpline;
		}

		//Getter
		bool& getClosedCurve()
		{
			return _closedCurve;
		}

		float& getCoefficient()
		{
			return _coefficient;
		}

		BSpline& getGeneralizedExtrudeBSpline()
		{
			return _bSplineExtrude;
		}

		// Génération de la nouvelle forme avec un extrude le long d'un axe (l'axe Z !)
		std::vector<Point> simpleExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve, const float height);
		// Génération de la nouvelle forme avec un extrude tourant autour d'un axe (l'axe Z !)
		std::vector<Point> revolvingExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve);
		// Génération de la nouvelle forme avec un extrude le long d'un B-Spline (B-Spline sur le plan z = 0 --> tous ses points doit avoir z = 0.00f !)
		std::vector<Point> generalizedExtrude(const std::vector<Point> bSplineCurve, const std::vector<Point> bSplineCurveController, const bool closedCurve);

		// Génération de la liste contenant l'indice des faces triangulaires à générer
		std::vector<int> generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int width, const unsigned int height);

		void drawCurve(std::vector<Point>& p, bool close = false);
		void drawTriangle(std::vector<Point>& p);
};

