#pragma once

using namespace std;

class Point
{
public:
    static int PointsCount;
    Point();
    Point(int, int);
    int x() { return (int)x_; };
    int y() { return (int)y_; };
    //friend ostream& operator<< (ostream& os, const Point& pt);
    bool operator== (Point& autre) const;
    bool estValide;
    bool enveloppe;

private:
    int x_, y_;
};

