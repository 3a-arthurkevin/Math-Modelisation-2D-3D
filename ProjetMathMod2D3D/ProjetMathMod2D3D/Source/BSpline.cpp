#include "../Include/BSpline.h"

BSpline::BSpline()
{
	_accuracy = 100.0f;
	_degree = 2;

	//_nodalVector = std::vector<float>();
	//_normalizedNodalVector = std::vector<float>();

	//_controlPoint = std::vector<Point>();
	//_bSplineCurve = std::vector<Point>();

	_closedBSpline = true;
}

BSpline::~BSpline()
{
}

void BSpline::addControlPoint(const Point point)
{
	_controlPoints.push_back(point);
	drawRect(point.getX(), point.getY(), 10);
}

//Rapport le vecteur nodal sur un interface [a,b] à un interval [0,1]
void BSpline::generateNormalizedNodalVector()
{
	/*
		[1---x---y---z---10] (schéma vecteur nodal)
		Pour rapporter une valeur x du vecteur nodal dans un interval [0, 1]
		Si les valeurs aux extrémités du vecteur nodal sont 1 et 10
		Alors le calcul à faire est ((x-1) / (10-1))
	*/
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
	/*
		On connait le nombre de points de controle (nbPC) et le degré (p)
		Soit n+1 le nombre de noeud dans le vecteur nodal (n est le nombre d'interval sur le vecteur nodal)
		D'après la formule du cours --> n - p = NbPC
		Donc n = NbPC + p
	*/
	unsigned int nbControlPoint = _controlPoints.size();

	if (nbControlPoint >= 2)
	{
		unsigned int nbNode = (nbControlPoint + _degree) + 1;

		_nodalVector.clear();
		_nodalVector.reserve(nbNode);

		for (unsigned int i = 0; i < nbNode; ++i)
			_nodalVector.push_back(i);

		generateNormalizedNodalVector();
	}
	else
		std::cout << "Erreur lors de la creation du vecteur nodal - Pas assez de point de controle" << std::endl;
}

// Fonction récursive effectuant l'algorithme deBoor
float BSpline::deBoor(int p, int i, float t)
{	
	/*
		Soit le degré p, l'index du noeud courant i et la valeur courante de parcourt du vecteur nodal t

		Si le degrée p est égal à 0 Alors
			Si t est compris entre la valeur du noeud courant et la valeur du noeud suivant
				retourner 1
			sinon 
				retourner 0
		Sinon (Récursion)
			On vérifie si il n'y a pas de valeur interdite au dénominateur pour calculer la Base B-Spline
			Si valeur interdite
				retourner 0
			sinon
				récussion (voir formule dans le cours pdf --> c'est compliquer/embetant à écrire)
	*/

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

	bool left = (tIPlusP - tI < 0.001f) && (tIPlusP - tI > -0.001f);
	bool right = (tIPlusOnePlusP - tIPlusOne < 0.001f) && (tIPlusOnePlusP - tIPlusOne > -0.001f);
	if (left || right)
		return 0;

	return (((t-tI)/(tIPlusP-tI)*deBoor(p-1, i, t)) + ((tIPlusOnePlusP-t)/(tIPlusOnePlusP-tIPlusOne)*deBoor(p-1, i+1, t)));
}

void BSpline::generateBSplineCurve()
{
	unsigned int nbControlPoint = _controlPoints.size();
	unsigned int nbInterval = _nodalVector.size() - 1;

	if (nbControlPoint > 0)
	{
		if (!_bSplineCurve.empty())
			_bSplineCurve.clear();

		if (nbInterval != (nbControlPoint + _degree))
		{
			generateVecteurNodal();
			nbInterval = _nodalVector.size() - 1;
		}

		float stepIncrement = 1.0f / _accuracy;

		float nodePValue = _normalizedNodalVector[_degree];
		float nodeNMinusPValue = _normalizedNodalVector[nbInterval - _degree];

		float bSplineBaseBuffer;

		float x, y, z;
		Point tmpControlPoint;

		for (float step = nodePValue; step <= nodeNMinusPValue; step += stepIncrement)
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

void BSpline::closeBSpline()
{
	/*
		Fermeture de la B-Spline seulement si on a 3 points au moins
		Pour fermer une courbe, le 1er et dernier point doivent etre identique
	*/

	int nbPoint = _bSplineCurve.size();

	if (nbPoint > 0)
	{
		if (_closedBSpline)
		{
			if (nbPoint > 3)
			{
				if (_bSplineCurve[0] != _bSplineCurve[nbPoint - 1])
					_bSplineCurve.push_back(_bSplineCurve[0]);
			}
			else
				std::cout << "Erreur fermeture B-spline - Impossible de fermer une forme avec moins de 3 points" << std::endl;
		}
		else
		{
			if (_bSplineCurve[0] == _bSplineCurve[nbPoint - 1])
				_bSplineCurve.pop_back();
		}
	}
	else
		std::cout << "Erreur fermeture B-spline - Impossible de fermer une forme sans points" << std::endl;
}

void BSpline::drawControlPoints(int length)
{
	for (auto it = _controlPoints.begin(); it != _controlPoints.end(); ++it)
		drawRect((*it).getX(), (*it).getY(), static_cast<float>(length));
}

void BSpline::drawBSplineCurve()
{
	if (_bSplineCurve.size() >= 3)
	{
		for (unsigned int i = 0; i < _bSplineCurve.size(); ++i)
		{
			if (i == _bSplineCurve.size() - 1)
			{
				if (_closedBSpline && i > 3)
					drawLine(_bSplineCurve[i], _bSplineCurve[0]);
				else
					break;
			}
			else
				drawLine(_bSplineCurve[i], _bSplineCurve[i + 1]);
		}
	}
	
}