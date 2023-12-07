#include<iostream>

using namespace std;

class Tacka2D {
	int x, y;
public:
	explicit Tacka2D(int x, int y) :x(x), y(y) {}
	bool operator==(Tacka2D other) const {
		if (x == other.x && y == other.y) return true;
		return false;
	}
	bool operator!=(Tacka2D other) const {
		return !(other == *this);
	}
	friend ostream& operator<<(ostream& os, const Tacka2D& t) {
		os << "x: " << t.x << " y: " << t.y << endl;
		return os;
	}
	Tacka2D operator+(Tacka2D& other) {
		return Tacka2D(x + other.x, y + other.y);
	}
};

int main() {

	Tacka2D t1(3, 4), t2(4, 3);
	Tacka2D t3 = t1 + t2;
	cout << t1 << t2 << t3;

	return 0;
}