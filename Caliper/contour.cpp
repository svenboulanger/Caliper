#include "contour.h"

void Contour::bounding_box(Point& min, Point& max)
{
	// Initialize to the opposite end
	min.x = min.y = numeric_limits<long>::max();
	max.x = max.y = numeric_limits<long>::lowest();

	// Go through all points and keep track of the max/min
	Contour::iterator i = begin();
	while (i != end()) {
		if (i->x < min.x)
			min.x = i->x;
		if (i->x > max.x)
			max.x = i->x;
		if (i->y < min.y)
			min.y = i->y;
		if (i->y > max.y)
			max.y = i->y;
		++i;
	}
}

ostream& operator<< (ostream& o, Contour& c)
{
	// Show number of indices
	o << c.vertex_count() << endl;

	// Show points in the contour
	for (Contour::iterator i = c.begin(); i != c.end(); i++)
		o << '\t' << i->x << " " << i->y << endl;
	return o;
}