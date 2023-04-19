#include "Point.h"

Point::Point(int x, int y) {
    x_ = x;
    y_ = y;
    estValide = x_ >= 0 && y_ >= 0 && x_ <= 7 && y_ <= 3;
    enveloppe = true;
}

Point::Point() {
    x_ = -1;
    y_ = -1;
    estValide = false;
    enveloppe = false;
}

//ostream& operator<<(ostream& os, const Point& pt) {
//    os << "x : " << pt.x_ << ", y : " << pt.y_;
//    return os;
//}

bool Point::operator== (Point& autre) const {
    return x_ == autre.x_ && y_ == autre.y_;
}