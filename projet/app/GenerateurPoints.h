#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Pos.h"
#include "Vecteur.h"


using namespace std;

#define LONGEUR_TABLE 4
#define LARGEUR_TABLE 8
#define MAX_POINTS 8


class GenerateurPoints
{
public:
    GenerateurPoints();
    enum Direction {
        NORD = 0,
        SUD = 4,
        EST = 2,
        OUEST = 6,
        NORD_EST = 1,
        NORD_OUEST = 7,
        SUD_EST = 3,
        SUD_OUESt = 5

    };
    void detecter(GenerateurPoints::Direction, int);
    void trierPoints();
    void enveloppeConvexe();
    void calculatePolygonArea();


    int area_;
    Pos position;
    Point points[MAX_POINTS];
    Point sorted_points[MAX_POINTS];
    Point enveloppe[MAX_POINTS];

};

