#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <algorithm>
#include <vector>

//#include "Includer.h"

#include "Point.h"
#include "BSpline.h"

class Extrude
{
	private:
		float _accuracy;
		bool _closedCurve;
		float _coefficient;
		BSpline _bSplineExtrude;

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

		std::vector<Point> simpleExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve, const float height);
		std::vector<Point> revolvingExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve);
		std::vector<Point> generalizedExtrude(const std::vector<Point> bSplineCurve, const std::vector<Point> bSplineCurveController, const bool closedCurve);

		std::vector<int> generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int width, const unsigned int height);
};

