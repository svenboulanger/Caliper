#pragma once

#include <vector>
#include "segment.h"

class Contour {
private:
	vector<Point> _points;
	vector<int> _holes;
public:
	typedef vector<Point>::iterator iterator;

	/// <summary>
	/// Gets a vertex at the specified index.
	/// </summary>
	/// <param name="p">The point index.</param>
	/// <returns>The point.</returns>
	Point& vertex(size_t p) { return _points[p]; }

	/// <summary>
	/// Gets a segment at the specified index.
	/// </summary>
	/// <param name="p">The point index.</param>
	/// <returns>The segment.</returns>
	Segment segment(size_t p) const { return (p == vertex_count() - 1) ? Segment(_points.back(), _points.front()) : Segment(_points[p], _points[p + 1]); }

	/// <summary>
	/// Gets the number of vertices in the contour.
	/// </summary>
	/// <returns>The number of vertices.</returns>
	size_t vertex_count() const { return _points.size(); }
	
	/// <summary>
	/// Gets the number of edges in the contour.
	/// </summary>
	/// <returns>The number of edges.</returns>
	size_t edge_count() const { return _points.size(); }

	/// <summary>
	/// Computes the bounding box of the contour.
	/// </summary>
	/// <param name="min">The first bounding corner point.</param>
	/// <param name="max">The second bounding corner point.</param>
	void bounding_box(Point& min, Point& max);

	/// <summary>
	/// Returns an iterator pointing to the first point in the contour.
	/// </summary>
	/// <returns>The iterator pointing to the first point in the contour.</returns>
	iterator begin() { return _points.begin(); }

	/// <summary>
	/// Returns an iterator pointing to the last point in the contour.
	/// </summary>
	/// <returns>The iterator pointing to the last point in the contour.</returns>
	iterator end() { return _points.end(); }

	/// <summary>
	/// Returns the last point in the contour.
	/// </summary>
	/// <returns>The last point in the contour.</returns>
	Point& back() { return _points.back(); }

	/// <summary>
	/// Returns the last point in the contour.
	/// </summary>
	/// <returns>The last point in the contour.</returns>
	const Point& back() const { return _points.back(); }

	/// <summary>
	/// Adds a point to the contour.
	/// </summary>
	/// <param name="s">The point.</param>
	void add(const Point& s) { _points.push_back(s); }
};

ostream& operator<< (ostream& o, Contour& c);