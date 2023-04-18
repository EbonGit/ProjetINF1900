#include "Pos.h"

Pos::Pos(int x, int y) {
	x_ = x;
	y_ = y;
}

Pos::Pos() {
    x_ = 0;
    y_ = 0;
}

void Pos::addX(int dist) {
    x_ += dist;
}

void Pos::addY(int dist) {
    y_ += dist;
}

bool Pos::estValide() {
    return x_ >= 0 && y_ >= 0 && x_ <= 7 && y_ <= 3;
}

//ostream& operator<<(ostream& os, const Pos& pos) {
//    os << "Position :" << " \n\t" << "x : " << pos.x_ << "\n\t" <<"y : " << pos.y_;
//    return os;
//}