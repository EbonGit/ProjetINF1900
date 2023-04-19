#pragma once
#include <math.h>
#include "Point.h"

using namespace std;
class Vecteur
{
public:
	Vecteur();
	Vecteur(Point, Point);
	Point points[2];
	int x() { return x_; };
	int y() { return y_; };
	//friend ostream& operator<< (ostream& os, const Vecteur& vec);
	bool operator== (Vecteur& autre) const;
	bool operator< (Vecteur& autre) const;
	static int crossProduct(Vecteur a, Vecteur b);

private:
	//coordinates
	int x_, y_;
	// radian angle to x axis
	double angleToHorizon;
	// length of the vector
	double longueur;
};

