#pragma once

#include <vector>

//#include "Includer.h"
#include "Point.h"
//#include "Extrude.h"


class BSpline
{
	private:
		float _accuracy;								// Précision lors de la génération du la B-SPline (parcours du vecteur nodal)
		int _degree;									// Degré de la B-Spline
		std::vector<int> _nodalVector;					// Vecteur nodal sur un interval [a, b] --> (a et b sont des réels)
		std::vector<float> _normalizedNodalVector;		// Vecteur nodal rapporté sur un interval [0, 1] (valeur réel entre 0.00f et 1.00f)
		std::vector<Point> _controlPoints;				// Points de controles servant à générer la B-Spline
		std::vector<Point> _bSplineCurve;				// B-Spline obtenue

		bool _closedBSpline;							// Décide si la B-Spline est une forme fermé ou ouverte

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

		//Ajoute un Point de controle à la liste des points de controles
		void addControlPoint(const Point point);
		//Génère la vecteur nodal arbitrairement selon le nombre de points de controles et le degré
		void generateVecteurNodal();
		//Génère la vecteur nodal sur l'interval [0, 1]
		void generateNormalizedNodalVector();

		// Algorithme deBoor générant un réel 
		float deBoor(int p, int i,float t);
		// Génère la courve B-Spline selon les points de controle, le vecteur nodal et le degré
		void generateBSplineCurve();

		//Fermeture ou ouvertue de la B-Spline
		void closeBSpline();
};