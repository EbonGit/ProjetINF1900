/* Auteurs: Kephren Delannay-Sampany, Jad Ben Rabhi, Amine Ghabia, Amine Zerouali
 * Description: 
 */

#include "GenerateurPoints.h"

int Point::PointsCount = 0;

GenerateurPoints::GenerateurPoints() : position(){
    area_ = 0;
    Point points[MAX_POINTS];
    Point sorted_points[MAX_POINTS];
    Point enveloppe[MAX_POINTS];
}

void GenerateurPoints::detecter(GenerateurPoints::Direction d, int dist) { //dist = 1 ou 2

    

    switch (d) {

    case Direction::NORD:
        position.addY(dist);
        if(position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::SUD:
        position.addY(-dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::EST:
        position.addX(dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::OUEST:
        position.addX(-dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::NORD_OUEST:
        position.addY(dist);
        position.addX(-dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::SUD_OUESt:
        position.addY(-dist);
        position.addX(-dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::NORD_EST:
        position.addY(dist);
        position.addX(dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    case Direction::SUD_EST:
        position.addY(-dist);
        position.addX(dist);
        if (position.estValide())
            points[Point::PointsCount] = Point(position.x(), position.y());
        break;

    }

    //cout << Point::PointsCount << " " << boolalpha << points[Point::PointsCount].estValide << endl;
    if(points[Point::PointsCount].estValide)
        Point::PointsCount++;
    
}

int compare(const void* a, const void* b)
{
    Vecteur vec_a = *((Vecteur*)a);
    Vecteur vec_b = *((Vecteur*)b);

    if (vec_a == vec_b) return 0;
    else if (vec_a < vec_b) return -1;
    else return 1;
}

// trie les points dans un tableau, les points non initialis�s sont � la fin
void GenerateurPoints::trierPoints() {

    // get lowest point
    Point lowest = points[0];
    for (size_t i = 1; i < MAX_POINTS; i++)
    {
        if (!points[i].estValide)
            continue;
        if (points[i].y() == lowest.y())
        {
            if (points[i].x() < lowest.x()) {
                lowest = points[i]; 
           }
        }
        else if (points[i].y() < lowest.y())
        {
            lowest = points[i]; 
        }
    }

    // create vectors
    Vecteur vectors[MAX_POINTS];
    for (size_t i = 0; i < MAX_POINTS; i++)
    {
        if (points[i] == lowest || !points[i].estValide)
            continue;
        vectors[i] = Vecteur(lowest, points[i]);

    }

    // sort the vectors
    qsort(vectors, MAX_POINTS, sizeof(Vecteur), compare);

    // sort the points
    sorted_points[0] = lowest;
    size_t index = 1;
    for (size_t i = 1; i < MAX_POINTS; i++)
    {
        if (vectors[i].points[1].estValide)
        {
            sorted_points[index] = vectors[i].points[1];
            index++;
        }
    }
}

// renvoie les points qui appartiennent � l'enveloppe convexe d'une liste de points
void GenerateurPoints::enveloppeConvexe() {
    // keep the index for the enveloppe list
    size_t index = 0;

    // add the first point
    enveloppe[index] = sorted_points[0];
    index++;

    // run though sorted points to determine number of actual points
    int number_of_points = 0;
    for (number_of_points; number_of_points < MAX_POINTS; number_of_points++)
    {
        if (sorted_points[number_of_points].estValide == false)
            break;
    }

    // array of vetors to compare
    Vecteur vectors[2];

    //loop through points and create two subsequent vectors
    int current_point = 1;
    while (current_point < number_of_points) {

        //check if current_point is on the enveloppe
        while (sorted_points[current_point].enveloppe == false)
            current_point++;// = (current_point + 1) % number_of_points;
        if (current_point == number_of_points)
            break;

        //get index of previous point
        int previous_point = current_point - 1;
        while (sorted_points[previous_point].enveloppe == false)
            previous_point = (previous_point - 1) % number_of_points;
        //if (previous_point < 0)
        //    break;

        // get index of next point
        int next_point = current_point + 1;
        while (sorted_points[next_point].enveloppe == false)
            next_point = (next_point + 1) % number_of_points;
        //if (next_point >= number_of_points)
        //    break;

        // create a vector between point 1 and 2
        vectors[0] = Vecteur(sorted_points[previous_point], sorted_points[current_point]);
        //DEBUG_PRINT("vector between " << sorted_points[previous_point] << " and " << sorted_points[current_point] << " = " << vectors[0] << endl);

        // create a vector between point 2 and 3
        vectors[1] = Vecteur(sorted_points[current_point], sorted_points[next_point]);
        //DEBUG_PRINT("vector between " << sorted_points[current_point] << " and " << sorted_points[next_point] << " = " << vectors[1] << endl);

        // calculate the cross product
        int c = Vecteur::crossProduct(vectors[0], vectors[1]);
        //DEBUG_PRINT("cross product between " << vectors[0] << " and " << vectors[1] << " = " << c << endl);

        // check if point 2 is in the enveloppe
        if (c >= 0) {
           // DEBUG_PRINT(sorted_points[current_point] << " is on the enveloppe" << "\n\n");
            // add point to the enveloppe
            enveloppe[index] = sorted_points[current_point];
            index++;
            current_point++;
        }
        else
        {
            sorted_points[current_point].enveloppe = false;
            //DEBUG_PRINT(sorted_points[current_point] << " is not on the enveloppe" << "\n\n");
            //current_point--;
        }
    }


}


void GenerateurPoints::calculatePolygonArea()
{
    int number_of_points = 0;

    for (size_t i = 0; i < MAX_POINTS; i++)
    {
        if (enveloppe[i].estValide == false)
            break;
        number_of_points++;
    }

    int j = number_of_points - 1;

    for (int i = 0; i < number_of_points; ++i) {
        area_ += ((enveloppe[j].x() + enveloppe[i].x()) * 11) * ((enveloppe[j].y() - enveloppe[i].y()) * 11);
        j = i;
    }

    area_ = abs(area_ / 2);
}
