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
	for (int i = 0; i < _n + 1; ++i)
	{
		for (int j = 0; j < _m + 1; ++j)
		{
			_surfaceControlPoints.push_back(Point(static_cast<float>(j*2), static_cast<float>(i*2), static_cast<float>(k)));
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

	int o=0, p;

	_surfaceBezierPoints.clear();

	for (int k = 0; k < _accuracy; ++k)
	{
		++o;
		p = 0;

		u = (k / _accuracy);
		for (int l = 0; l < _accuracy; ++l)
		{
			++p;

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

	std::vector<Point> tmpPoint = _surfaceBezierPoints;

	std::vector<int> indexVertices = generateTriangularFacesIndex(tmpPoint, o, p);
	_surfaceBezierPoints.clear();

	for (auto it = indexVertices.begin(); it != indexVertices.end(); ++it)
		_surfaceBezierPoints.push_back(tmpPoint[*it]);

}

std::vector<int> BezierSurface::generateTriangularFacesIndex(const std::vector<Point> shape, const unsigned int width, const unsigned int height)
{
	/*
	Lors de la génération des points d'une forme
	Les points sont fait lignes par ligne c'est à dire

	Exemple face composé de 2 triangles
	._____.
	|   / |
	| /   |
	._____.

	On parcours ligne par ligne (on applique un -1 à width et height dans les boucles pour ne pas sortir du tableau !)
	Opération bizarre pour obtenir les sommets parce qu'on est dans un tableau 1D contenant tous les points (pareil que pour les matrices)
	*/

	std::vector<int> triangluarFacesIndex;

	int index1, index2, index3;

	for (unsigned int i = 0; i < height - 2; ++i)
	{
		for (unsigned int j = 0; j < width - 2; ++j)
		{
			index1 = ((i    * (width + 1)) + j);
			index2 = (((i + 1) * (width + 1)) + j);
			index3 = (((i + 1) * (width + 1)) + (j + 1));
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

			index1 = ((i    * (width + 1)) + j);
			index2 = (((i + 1) * (width + 1)) + (j + 1));
			index3 = ((i    * (width + 1)) + (j + 1));
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

void BezierSurface::drawSurface()
{
	for (unsigned int i = 0; i < this->_surfaceBezierPoints.size(); ++i)
	{
		if (i == this->_surfaceBezierPoints.size() - 1)
		{
			break;
		}
		else
			drawLine(this->_surfaceBezierPoints[i], this->_surfaceBezierPoints[i + 1]);
	}
}

void BezierSurface::drawControlPoints(int length)
{
	for (auto it = this->_surfaceControlPoints.begin(); it != this->_surfaceControlPoints.end(); ++it)
		drawRect((*it), static_cast<float>(1));
}