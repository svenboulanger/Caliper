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

/// <summary>
/// Finds the intersection between two segments.
/// </summary>
/// <param name="seg0">The first segment.</param>
/// <param name="seg1">The second segment.</param>
/// <param name="pi0">The first intersection point.</param>
/// <param name="pi1">The second intersection point, if any.</param>
/// <returns>Returns 0 if there is no intersection; 1 if there is one intersection and 2 if there is an overlapping segment.</returns>
int find_intersection(const Segment& seg0, const Segment& seg1, Point& pi0, Point& pi1);

inline ostream& operator<< (ostream& o, const Segment& p) {
	return o << p.begin() << "-" << p.end();
}