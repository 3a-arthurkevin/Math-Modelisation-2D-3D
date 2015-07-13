#include "../Include/BezierSurface.h"


BezierSurface::BezierSurface()
{
	_accuracy = 10.00f;
	_m = 3;
	_n = 3;
	//_controlPoints = std::vector<Point>();
	//_bezierPoints = std::vector<Point>();

	generateSurfaceControlPoints();
}


BezierSurface::~BezierSurface()
{
}

void BezierSurface::generateSurfaceControlPoints()
{
	int k = 1;
	for (int i = 0; i < _n+1; ++i)
	{
		for (int j = 0; j < _m+1; ++j)
		{
			_surfaceControlPoints.push_back(Point(static_cast<float>(j), static_cast<float>(i), static_cast<float>(k)));
			k += (-k * 2);
		}
	}
}


Point BezierSurface::deCaseljou(std::vector<Point> curve, float step)
{
	/*
		Voit cours et/ou ancien TP
	*/

	int level = curve.size();

	std::vector<Point> tmpPreviousLevelPoints, tmpCurrentLevelPoints;

	tmpPreviousLevelPoints = curve;

	int i, j, totalPoints;
	for (i = 1; i < level; ++i)
	{
		totalPoints = tmpPreviousLevelPoints.size();
		for (j = 0; j < (totalPoints - 1); ++j)
			tmpCurrentLevelPoints.push_back((tmpPreviousLevelPoints[j] * (1.0f - step)) + (tmpPreviousLevelPoints[j + 1] * step));

		tmpPreviousLevelPoints = tmpCurrentLevelPoints;
		tmpCurrentLevelPoints.clear();
	}

	return tmpPreviousLevelPoints[0];
}

void BezierSurface::generateBezierSurface()
{
	/*
		On a notre espace de paramétrage comprit dans l'interval [0, 1] pour u et v
		Donc en parcourant cet espace pour u et v selon le pas défini
		on applique la formule du cours pour avec les points de la surface de Bezier

		Voir formule cours --> c'est compliquer à expliquer en commentaire détailé
	*/

	std::vector<Point> intermediaryRowControlPoints;
	std::vector<Point> intermediaryBezierPoints;
	float u, v;

	_surfaceBezierPoints.clear();

	for (int k = 0; k < _accuracy; ++k)
	{
		u = (k / _accuracy);
		for (int l = 0; l < _accuracy; ++l)
		{
			v = (l / _accuracy);
			for (int i = 0; i < _n + 1; ++i)
			{
				//Récupération des points d'une ligne pour un cetrain colonne
				for (int j = 0; j < _m + 1; ++j)
					intermediaryRowControlPoints.push_back(_surfaceControlPoints[i + j]);

				//Ajout des points de Bezier intermédiaire
				intermediaryBezierPoints.push_back(deCaseljou(intermediaryRowControlPoints, v));
				intermediaryRowControlPoints.clear();
			}
			//Ajout des points de la surface de Bezier selon les points de Bezier intermidiaire
			_surfaceBezierPoints.push_back(deCaseljou(intermediaryBezierPoints, u));
			intermediaryBezierPoints.clear();
		}
	}
}
