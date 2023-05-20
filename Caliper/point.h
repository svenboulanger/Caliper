#pragma once

#include <iostream>
using namespace std;

/// <summary>
/// A 2D point.
/// </summary>
class Point {
public:
	/// <summary>
	/// The x-coordinate.
	/// </summary>
	long x;

	/// <summary>
	/// The y-coordinate.
	/// </summary>
	long y;

	/// <summary>
	/// Constructor.
	/// </summary>
	Point() :
		x(0), y(0) {}

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="ax">The x-coordinate.</param>
	/// <param name="ay">The y-coordinate.</param>
	Point(long ax, long ay) :
		x(ax), y(ay) {}

	/// <summary>
	/// Overrides the equality operator between points.
	/// </summary>
	/// <param name="p">The other point.</param>
	/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
	bool operator== (const Point& p) const { return x == p.x && y == p.y; }

	/// <summary>
	/// Override the inequality operator between points.
	/// </summary>
	/// <param name="p">The other point.</param>
	/// <returns>Returns <c>true</c> if both are not equal; otherwise, <c>false</c>.</returns>
	bool operator!= (const Point& p) const { return x != p.x || y != p.y; }

	const Point operator+ (const Point& b) const { return Point(x + b.x, y + b.y); }

	const Point operator- (const Point& b) const { return Point(x - b.x, y - b.y); }
	
	Point operator+ (Point b) { return Point(x + b.x, y + b.y); }

	Point operator- (Point b) { return Point(x - b.x, y - b.y); }
};

inline ostream& operator<< (ostream& o, const Point& p)
{
	return o << "(" << p.x << "," << p.y << ")";
}
inline istream& operator>> (istream& i, Point& p)
{
	return i >> p.x >> p.y;
}