#include "polygon.h"
using namespace std;

void Polygon::bounding_box(Point& min, Point& max)
{
	// Initialize to opposite end
	min.x = min.y = numeric_limits<long>::max();
	max.x = max.y = numeric_limits<long>::lowest();

	// Go through all contours and track the min/max
	Point cmin, cmax;
	for (Polygon::iterator i = begin(); i != end(); i++) {
		i->bounding_box(cmin, cmax);

		if (cmin.x < min.x)
			min.x = cmin.x;
		if (cmax.x > max.x)
			max.x = cmax.x;
		if (cmin.y < min.y)
			min.y = cmin.y;
		if (cmax.y > max.y)
			max.y = cmax.y;
	}
}

ostream& operator<< (ostream& o, Polygon& p)
{
	// Number of contours
	o << p.contour_count() << endl;

	// Write the contours themselves
	for (Polygon::iterator i = p.begin(); i != p.end(); i++)
		o << *i;

	// Write the holes of every contour

	return o;
}