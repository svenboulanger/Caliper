#pragma once

#include "point.h"

class Segment {
public:
	/// <summary>
	/// The start point.
	/// </summary>
	Point p1;

	/// <summary>
	/// The end point.
	/// </summary>
	Point p2;

	/// <summary>
	/// Constructor.
	/// </summary>
	Segment() {}

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="ap1">The first point.</param>
	/// <param name="ap2">The second point.</param>
	Segment(const Point& ap1, const Point& ap2) :
		p1(ap1), p2(ap2) {}

	/// <summary>
	/// Gets the starting point of the segment.
	/// </summary>
	/// <returns>The start point.</returns>
	const Point& begin() const { return p1; }

	/// <summary>
	/// Gets the ending point of the segment.
	/// </summary>
	/// <returns>The end point.</returns>
	const Point& end() const { return p2; }

	/// <summary>
	/// Flips the orientation of the segment.
	/// </summary>
	void changeOrientation()
	{
		Point tmp = p1;
		p1 = p2;
		p2 = tmp;
	}
};

inline ostream& operator<< (ostream& o, const Segment& p) {
	return o << p.begin() << "-" << p.end();
}