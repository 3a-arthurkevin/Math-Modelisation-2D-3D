#pragma once

#include <vector>

//#include "Includer.h"
#include "Point.h"
//#include "Extrude.h"


class BSpline
{
	private:
		float _accuracy;								// Pr�cision lors de la g�n�ration du la B-SPline (parcours du vecteur nodal)
		int _degree;									// Degr� de la B-Spline
		std::vector<int> _nodalVector;					// Vecteur nodal sur un interval [a, b] --> (a et b sont des r�els)
		std::vector<float> _normalizedNodalVector;		// Vecteur nodal rapport� sur un interval [0, 1] (valeur r�el entre 0.00f et 1.00f)
		std::vector<Point> _controlPoints;				// Points de controles servant � g�n�rer la B-Spline
		std::vector<Point> _bSplineCurve;				// B-Spline obtenue

		bool _closedBSpline;							// D�cide si la B-Spline est une forme ferm� ou ouverte

		//Extrude _bSplineExtruder;

	public:
		//Constructor & Destructor
		BSpline();
		~BSpline();

		//getter
		void setAccuracy(float value)
		{
			_accuracy = value;
		}

		void setDeDegree(float value)
		{
			_degree = _degree;
		}

		void setNodalVector(std::vector<int> nodalVector)
		{
			_nodalVector = nodalVector;
		}

		void setNormalizedNodalVector(std::vector<float> normalizedNodalVector)
		{
			_normalizedNodalVector = normalizedNodalVector;
		}

		void setControlPoint(std::vector<Point> controlPoint)
		{
			_controlPoints = controlPoint;
		}

		void setBSplineCurve(std::vector<Point> bSplineCurve)
		{
			_bSplineCurve = bSplineCurve;
		}

		void setClosedBSpline(bool value)
		{
			_closedBSpline = value;
		}

		//void setBSplineExtruder(Extrude extrude)
		//{
		//	_bSplineExtruder = extrude;
		//}


		//Getter
		float& getAccuracy()
		{
			return _accuracy;
		}

		int& getDeDegree()
		{
			_degree = _degree;
		}

		std::vector<int>& getNodalVector()
		{
			return _nodalVector;
		}

		std::vector<float>& getNormalizedNodalVector()
		{
			return _normalizedNodalVector;
		}

		std::vector<Point>& getControlPoint()
		{
			return _controlPoints;
		}

		std::vector<Point>& getBSplineCurve()
		{
			return _bSplineCurve;
		}

		bool& getClosedBSpline()
		{
			return _closedBSpline;
		}

		//Extrude& getBSplineExtruder()
		//{
		//	return _bSplineExtruder;
		//}

		//Ajoute un Point de controle � la liste des points de controles
		void addControlPoint(const Point point);
		//G�n�re la vecteur nodal arbitrairement selon le nombre de points de controles et le degr�
		void generateVecteurNodal();
		//G�n�re la vecteur nodal sur l'interval [0, 1]
		void generateNormalizedNodalVector();

		// Algorithme deBoor g�n�rant un r�el 
		float deBoor(int p, int i,float t);
		// G�n�re la courve B-Spline selon les points de controle, le vecteur nodal et le degr�
		void generateBSplineCurve();

		//Fermeture ou ouvertue de la B-Spline
		void closeBSpline();
};