#include "../Include/BSpline.h"

BSpline::BSpline()
{
	_accuracy = 20.0f;
	_degree = 3;

	//_nodalVector = std::vector<float>();
	//_normalizedNodalVector = std::vector<float>();

	//_controlPoint = std::vector<Point>();
	//_bSplineCurve = std::vector<Point>();
}


BSpline::~BSpline()
{
}

void BSpline::addControlPoint(const Point point)
{
	_controlPoints.push_back(point);
}

//Rapport le vecteur nodal sur un interface [a,b] à un interval [0,1]
void BSpline::generateNormalizedNodalVector()
{
	unsigned int nbNode = _nodalVector.size();

	if (nbNode >= 2)
	{	
		int firstNodeValue = _nodalVector[0];
		int lastNodeValue = _nodalVector[nbNode - 1];

		float lastMinusFirst = static_cast<float>(lastNodeValue - firstNodeValue);

		_normalizedNodalVector.clear();
		_normalizedNodalVector.reserve(nbNode);

		for (unsigned int i = 0; i < nbNode; ++i)
			_normalizedNodalVector.push_back(((_nodalVector[i] - firstNodeValue) / (lastMinusFirst)));
	}
	else
		std::cout << "Erreur lors de la création du vector nodal normalise - Pas assez de noeud" << std::endl;
}

// Créer un vecteur nodal par défaut
void BSpline::generateVecteurNodal()
{
	unsigned int nbControlPoint = _controlPoints.size();

	if (nbControlPoint >= 2)
	{
		unsigned int nbNode = (nbControlPoint + _degree) + 1;

		_nodalVector.clear();
		_nodalVector.reserve(nbNode);

		for (unsigned int i = 0; i < nbNode; ++i)
			this->_nodalVector.push_back(i);

		generateNormalizedNodalVector();
	}
	else
		std::cout << "Erreur lors de la creation du vecteur nodal - Pas assez de point de controle" << std::endl;
}

float BSpline::deBoor(int p, int i, float t)
{	
	if (p == 0)
	{
		if ((t >= _normalizedNodalVector[i]) && (t < _normalizedNodalVector[i + 1]))
			return 1.00f;
		return 0.00f;
	}

	float tI = _normalizedNodalVector[i];
	float tIPlusP = _normalizedNodalVector[i + p];

	float tIPlusOnePlusP = _normalizedNodalVector[i + 1 + p];
	float tIPlusOne = _normalizedNodalVector[i + 1];

	bool left = (tIPlusP - tI < 0.000001) && (tIPlusP - tI > -0.000001);
	bool right = (tIPlusOnePlusP - tIPlusOne < 0.000001) && (tIPlusOnePlusP - tIPlusOne > -0.000001);
	if (left || right)
		return 0;

	return (((t-tI)/(tIPlusP-tI)*deBoor(p-1, i, t)) + ((tIPlusOnePlusP-t)/(tIPlusOnePlusP-tIPlusOne)*deBoor(p-1, i+1, t)));
}

void BSpline::generateBSplineCurve()
{
	unsigned int nbControlPoint = _controlPoints.size();
	unsigned int nbNode = _nodalVector.size();

	if (nbControlPoint > 0)
	{
		if (!_bSplineCurve.empty())
			_bSplineCurve.clear();

		if (nbNode != (nbControlPoint + _degree + 1))
			generateVecteurNodal();

		float stepIncrement = 1.0f / _accuracy;

		float nodePValue = _normalizedNodalVector[_degree];
		float nodeNMinusPMinusOneValue = _normalizedNodalVector[nbNode - _degree - 1];

		float bSplineBaseBuffer;

		float x, y, z;
		Point tmpControlPoint;

		for (float step = nodePValue; step <= nodeNMinusPMinusOneValue; step += stepIncrement)
		{
			bSplineBaseBuffer = 0.00f;
			x = 0.00f;
			y = 0.00f;
			z = 0.00f;

			for (unsigned int indexPoint = 0; indexPoint < nbControlPoint; ++indexPoint)
			{
				tmpControlPoint = _controlPoints[indexPoint];

				bSplineBaseBuffer = deBoor(_degree, indexPoint, step);
				
				x += (bSplineBaseBuffer * tmpControlPoint.getX());
				y += (bSplineBaseBuffer * tmpControlPoint.getY());
				z += (bSplineBaseBuffer * tmpControlPoint.getZ());
			}

			_bSplineCurve.push_back(Point(x, y, z));
		}
	}
	else
		std::cout << "Erreur lors de la creation du la B-spline - Pas assez de point de controle" << std::endl;
}