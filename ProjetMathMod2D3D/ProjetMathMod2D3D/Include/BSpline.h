#pragma once

#include <vector>

#include "Point.h"

class BSpline
{
	private:
		float _accuracy;
		int _degree;
		std::vector<int> _nodalVector;
		std::vector<float> _normalizedNodalVector;
		std::vector<Point> _controlPoints;
		std::vector<Point> _bSplineCurve;

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

		//Getter
		float getAccuracy()
		{
			return _accuracy;
		}

		int getDeDegree()
		{
			_degree = _degree;
		}

		std::vector<int> getNodalVector()
		{
			return _nodalVector;
		}

		std::vector<float> getNormalizedNodalVector()
		{
			return _normalizedNodalVector;
		}

		std::vector<Point> getControlPoint()
		{
			return _controlPoints;
		}

		std::vector<Point> getBSplineCurve()
		{
			return _bSplineCurve;
		}


		void addControlPoint(const Point point);
		void generateVecteurNodal();
		void generateNormalizedNodalVector();

		float deBoor(int p, int i,float t);
		void generateBSplineCurve();
		/*
			p = 3
			n = 10 (On a n+1 noeuds dans le vecteur nodal)

			n-p = 7 points de controle (P0 à P6) 
			n+1 = 11 noeuds dans le vecteur nodal
		*/

		/*
			p = degré BSpline
			pas : ti+1 = ti + pas
			n-p = nb Point controle (On a n+1 noeud sur le vecteur nodal)
			n+1 = nb Noeud vecteur nodal
			i = index vecteur nodal --> 0 <= i <= n-p-i

			B = Base B-Spline --> B != 0 sur [Noeud(i), Noeud(i+1+p)[
		*/

		/*
			Boucler avec tp <= t <= tn-p --> partition de l'unité
			Pas : ti+1 = ti + pas
		*/
};