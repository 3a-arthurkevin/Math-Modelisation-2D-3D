#include "../Include/Extrude.h"

Extrude::Extrude()
{
	_accuracy = 10.00f;
	_closedCurve = true;
	_coefficient = 0.00f;

	unsigned int capacity = 6;
	_bSplineExtrude.getControlPoint().reserve(capacity);

	for (unsigned int i = 0; i < capacity; ++i)
		_bSplineExtrude.getControlPoint().push_back(Point(static_cast<float>(i), 0.00f, 0.00f));

	_bSplineExtrude.generateBSplineCurve();
}


Extrude::~Extrude()
{
}

std::vector<Point> Extrude::simpleExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve, const float height)
{
	/*
		Extrusion le long de l'axe Z
		On extude vers le bas pas après pas sur une hauteur déterminer
		On extrude de 0 à height
		On applique un coefficient d'agrandissement ou réduction lors de l'extude
	*/

	std::vector<Point> shape;

	float stepIncrement = 1.00f / _accuracy;

	unsigned int nbPoint = bSplineCurve.size();
	Point tmpPoint;
	float x, y, z;

	int i = 0;

	for (float t = 0.00f; t <= 1.00f; t+=stepIncrement)
	{
		for (unsigned int s = 0; s < nbPoint; ++s)
		{
			tmpPoint = bSplineCurve[s];

			x = tmpPoint.getX() * ((t * (_coefficient - 1.00f)) + 1);
			y = tmpPoint.getY() * ((t * (_coefficient - 1.00f)) + 1);
			z = t * height;

			shape.push_back(Point(x, y, z));
		}
		++i;
	}
	
	std::vector<int> indexVertices = generateTriangularFacesIndex(shape, bSplineCurve.size(), i);
	std::vector<Point> shapedToReturn;
	for (auto it = indexVertices.begin(); it != indexVertices.end(); ++it)
	{
		shapedToReturn.push_back(shape[*it]);
	}

	return shapedToReturn;
	
	return shape;
}

std::vector<Point> Extrude::revolvingExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve)
{
	/*
		Extrusion au tour de l'axe Z
		On décale de pas en pas (un pas équivaux à un agrandissement de l'angle)
		On extrude de 0 à 360° --> de 0 à 2Pi
	*/

	std::vector<Point> shape;

	float angleIncrement = 1.00f / _accuracy;

	unsigned int nbPoint = bSplineCurve.size();
	Point tmpPoint;
	float x, y, z;

	int nbMotif = 0;

	for (float teta = 0.00f; teta <= (2 * M_PI); teta += angleIncrement)
	{
		for (unsigned int s = 0; s < nbPoint; ++s)
		{
			tmpPoint = bSplineCurve[s];

			z = tmpPoint.getX() * cos(teta);
			x = tmpPoint.getX() * sin(teta);
			y = tmpPoint.getY();

			shape.push_back(Point(x, y, z));
		}
		++nbMotif;
	}
	
	std::vector<int> indexVertices = generateTriangularFacesIndex(shape, bSplineCurve.size(), nbMotif);
	std::vector<Point> shapedToReturn;
	for (auto it = indexVertices.begin(); it != indexVertices.end(); ++it)
	{
		shapedToReturn.push_back(shape[*it]);
	}

	return shapedToReturn;
	
	return shape;
}

std::vector<Point> Extrude::generalizedExtrude(const std::vector<Point> bSplineCurve, const std::vector<Point> bSplineCurveController, const bool closedCurve)
{
	/*
		La courbe B-Spline à extrude suit l'autre courbe B-Spline de controle
		Pour faire cette extrude, on doit parcourir la B-Spline de controle point par point
			Récupérer le vecteur tangeant à chaque pas
				(Cas particulier pour le 1er et dernier points)
			Récupérer le vecteur normal à partir du vecteur tangeant pour le calcule
				(Le vecteur k est connu car la B-Spline de controle est inclu dans le plan z = 0 --> tous les points de la B-Spline de controle on z = 0)
	
	*/
	std::vector<Point> shape;
	Point pointToAdd;

	unsigned int nbPoint = bSplineCurve.size();
	unsigned int nbPointController = bSplineCurveController.size();
	
	Point tmpPoint, tmpPointController;
	Point tangentVector, normalVector;
	Point kVector(0.00f, 0.00f, 1.00f);

	for (unsigned int t = 0; t < nbPointController; ++t)
	{
		tmpPointController = bSplineCurveController[t];

		if (t == 0)
			tangentVector = ((bSplineCurveController[t + 1] - bSplineCurveController[t]) /* / 1 */);
		else if (t == (nbPointController - 1))
			tangentVector = ((bSplineCurveController[t + 1] - bSplineCurveController[t - 1]) / 2);
		else
			tangentVector = ((bSplineCurveController[t - 1] - bSplineCurveController[t]) /* / 1 */);

		tangentVector /= tangentVector.norm();

		normalVector = Point( ((tangentVector.getY() * kVector.getZ()) - (tangentVector.getZ() * kVector.getY())), 
							  ((tangentVector.getZ() * kVector.getX()) - (tangentVector.getX() * kVector.getZ())),
							  ((tangentVector.getX() * kVector.getY()) - (tangentVector.getY() * kVector.getX())) );

		for (unsigned int s = 0; s < nbPoint; ++s)
		{
			tmpPoint = bSplineCurve[s];
			pointToAdd = tmpPointController + (normalVector * tmpPoint.getX()) + (kVector * tmpPoint.getY());
			shape.push_back(pointToAdd);
		}
	}

	return shape;
}

std::vector<int> Extrude::generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int nbMotif, const unsigned int nbPointMotif)
{
	/*
		Lors de la génération des points d'une forme
		Les points sont fait lignes par ligne c'est à dire 

		Exemple face composé de 2 triangles
		._____.
		|   / |
		| /   |
		._____.

		On parcours ligne par ligne (on applique un -1 à nbMotif et nbPointMotif dans les boucles pour ne pas sortir du tableau !)
		Opération bizarre pour obtenir les sommets parce qu'on est dans un tableau 1D contenant tous les points (pareil que pour les matrices)
	*/

	std::vector<int> triangluarFacesIndex;

	int index1, index2, index3;

	for (unsigned int i = 0; i < nbMotif; ++i)
	{
		for (unsigned int j = 0; j < nbPointMotif; ++j)
		{
			index1 = ( (   j    * (nbPointMotif + 1)) +    i    );
			index2 = ( ((j + 1) * (nbPointMotif + 1)) +    i    );
			index3 = ( ((j + 1) * (nbPointMotif + 1)) + (i + 1) );
			/*
			         (index3)
                        .
					  / |
					/   |
			      ._____. 
			(index1)   (index2)
			*/

			triangluarFacesIndex.push_back(index1);
			triangluarFacesIndex.push_back(index2);
			triangluarFacesIndex.push_back(index3);

			index1 = ( (   j    * (nbPointMotif + 1)) +    i    );
			index2 = ( ((j + 1) * (nbPointMotif + 1)) + (i + 1) );
			index3 = ( (   j    * (nbPointMotif + 1)) + (i + 1) );
			/*
			 (index3)   (index2)
                  ._____.
			      |   /
				  | /
			      .
			   (index1)   
			*/

			triangluarFacesIndex.push_back(index1);
			triangluarFacesIndex.push_back(index2);
			triangluarFacesIndex.push_back(index3);
		}
	}

	return triangluarFacesIndex;
}

void Extrude::drawCurve(std::vector<Point>& p, bool close)
{
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		if (i == p.size() - 1)
		{
			if (close)
				drawLine(p[i], p[0]);
			else
				break;
		}
		else
			drawLine(p[i], p[i + 1]);
	}
}
void Extrude::drawTriangle(std::vector<Point>& p)
{
	for (unsigned int i = 0; i < p.size(); ++i)
	{
		if (i == p.size() - 1)
		{
			drawLine(p[i], p[0]);
		}
		else
			drawLine(p[i], p[i + 1]);
	}
}