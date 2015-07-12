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
	std::vector<Point> shape;

	float stepIncrement = 1.00f / _accuracy;

	unsigned int nbPoint = bSplineCurve.size();
	Point tmpPoint;
	float x, y, z;

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
	}

	return shape;
}

std::vector<Point> Extrude::revolvingExtrude(const std::vector<Point> bSplineCurve, const bool closedCurve)
{
	std::vector<Point> shape;

	float angleIncrement = 1.00f / _accuracy;

	unsigned int nbPoint = bSplineCurve.size();
	Point tmpPoint;
	float x, y, z;

	for (float teta = 0.00f; teta <= (2 * M_PI); teta += angleIncrement)
	{
		for (unsigned int s = 0; s < nbPoint; ++s)
		{
			tmpPoint = bSplineCurve[s];

			x = tmpPoint.getX() * cos(teta);
			y = tmpPoint.getX() * sin(teta);
			z = tmpPoint.getY();

			shape.push_back(Point(x, y, z));
		}
	}

	return shape;
}

std::vector<Point> Extrude::generalizedExtrude(const std::vector<Point> bSplineCurve, const std::vector<Point> bSplineCurveController, const bool closedCurve)
{
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

std::vector<int> Extrude::generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int width, const unsigned int height)
{

	std::vector<int> triangluarFacesIndex;

	int index1, index2, index3;

	for (unsigned int i = 0; i < width - 1; ++i)
	{
		for (unsigned int j = 0; j < height - 1; ++j)
		{
			index1 = ( (   i    * (height + 1)) +    j    );
			index2 = ( ((i + 1) * (height + 1)) +    j    );
			index3 = ( ((i + 1) * (height + 1)) + (j + 1) );

			triangluarFacesIndex.push_back(index1);
			triangluarFacesIndex.push_back(index2);
			triangluarFacesIndex.push_back(index3);

			index1 = ( (   i    * (height + 1)) +    j    );
			index2 = ( ((i + 1) * (height + 1)) + (j + 1) );
			index3 = ( (   i    * (height + 1)) + (j + 1) );

			triangluarFacesIndex.push_back(index1);
			triangluarFacesIndex.push_back(index2);
			triangluarFacesIndex.push_back(index3);
		}
	}

	return triangluarFacesIndex;
}
