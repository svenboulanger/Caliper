#pragma once

#include <vector>
#include "contour.h"

class Polygon {
private:
	vector<Contour> _contours;

public:
	typedef vector<Contour>::iterator iterator;

	/// <summary>
	/// Constructor.
	/// </summary>
	Polygon()
		: _contours()
	{}

	/// <summary>
	/// Gets the contour at the specified index.
	/// </summary>
	/// <param name="p">The index.</param>
	/// <returns>The contour at the specified index.</returns>
	Contour& contour(unsigned p) { return _contours[p]; }

	/// <summary>
	/// Gets the contour at the specified index.
	/// </summary>
	/// <param name="p">The index.</param>
	/// <returns>The contour at the specified index.</returns>
	Contour& operator[] (unsigned p) { return _contours[p]; }

	/// <summary>
	/// Gets the number of contours in the polygon.
	/// </summary>
	/// <returns>The number of contours.</returns>
	size_t contour_count() const { return _contours.size(); }

	/// <summary>
	/// Computes the bounding box of the contour.
	/// </summary>
	/// <param name="min">The first bounding corner point.</param>
	/// <param name="max">The second bounding corner point.</param>
	void bounding_box(Point& min, Point& max);

	/// <summary>
	/// Returns an iterator pointing to the first contour in the polygon.
	/// </summary>
	/// <returns>The iterator pointing to the first contour in the polygon.</returns>
	iterator begin() { return _contours.begin(); }

	/// <summary>
	/// Returns an iterator pointing to the last contour in the polygon.
	/// </summary>
	/// <returns>The iterator pointing to the last contour in the polygon.</returns>
	iterator end() { return _contours.end(); }

	/// <summary>
	/// Gets the last contour in the polygon.
	/// </summary>
	/// <returns>The last contour.</returns>
	Contour& back() { return _contours.back(); }

	/// <summary>
	/// Gets the last contour in the polygon.
	/// </summary>
	/// <returns>The last contour.</returns>
	const Contour& back() const { return _contours.back(); }

	/// <summary>
	/// Adds a contour to the end of the polygon.
	/// </summary>
	/// <param name="c">The contour.</param>
	void push_back(const Contour& c) { _contours.push_back(c); }

	/// <summary>
	/// Removes the item specified.
	/// </summary>
	/// <param name="i">The iterator pointint to the contour being erased.</param>
	void erase(iterator i) { _contours.erase(i); }

	/// <summary>
	/// Clears the polygon of any contours.
	/// </summary>
	void clear() { _contours.clear(); }
};

ostream& operator<< (ostream& o, Polygon& p);