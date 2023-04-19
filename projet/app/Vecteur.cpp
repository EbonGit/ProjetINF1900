#include "Vecteur.h"

Vecteur::Vecteur() {
	x_ = 0;
	y_ = 0;
	longueur = -1;
	angleToHorizon = -1;
}

Vecteur::Vecteur(Point a, Point b) {
	points[0] = a;
	points[1] = b;
	
	x_ = b.x() - a.x();
	y_ = b.y() - a.y();

	longueur = sqrt(pow(x_, 2) + pow(y_, 2));

	angleToHorizon = atan2f(y_, x_);
}

//ostream& operator<< (ostream& os, const Vecteur& vec) {
	//return os << "vector : (" << vec.x_ << ", " << vec.y_ << ")\n" 
	//	<< "  longueur : " << vec.longueur << endl  
	//	<< "  angle de " << vec.angleToHorizon << endl;
	//return os << "vector : (" << vec.x_ << ", " << vec.y_ << ")";
//}

bool Vecteur::operator== (Vecteur& autre) const {
	return angleToHorizon == autre.angleToHorizon;
}

bool Vecteur::operator< (Vecteur& autre) const {
	return angleToHorizon < autre.angleToHorizon;
}

int Vecteur::crossProduct(Vecteur a, Vecteur b) {
	return (a.x() * b.y()) - (a.y() * b.x());
}