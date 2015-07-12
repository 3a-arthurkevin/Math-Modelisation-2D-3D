#pragma once

#include <vector>

//#include "Includer.h"
#include "Point.h"
//#include "Extrude.h"


class BSpline
{
	private:
		float _accuracy;
		int _degree;
		std::vector<int> _nodalVector;
		std::vector<float> _normalizedNodalVector;
		std::vector<Point> _controlPoints;
		std::vector<Point> _bSplineCurve;

		bool _closedBSpline;

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

		void addControlPoint(const Point point);
		void generateVecteurNodal();
		void generateNormalizedNodalVector();

		float deBoor(int p, int i,float t);
		void generateBSplineCurve();

		void closeBSpline();
};