#pragma once

using namespace std;

class Pos
{

public:
	Pos();
	Pos(int, int);
	int x() { return x_; };
	int y() { return y_; }; 
	void addY(int incr);
	void addX(int incr);
	bool estValide();
	//friend ostream& operator<< (ostream& os, const Pos& pt);

private:
	int x_, y_;
};

