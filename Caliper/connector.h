#pragma once

#include "segment.h"
#include "martinez.h"
#include <list>

class PointChain
{
public:
	typedef list<Point>::iterator iterator;
	
	/// <summary>
	/// Constructor.
	/// </summary>
	PointChain()
		: _l(), _closed(false)
	{
	}

	/// <summary>
	/// Initializes the point chain with the given segment.
	/// </summary>
	/// <param name="s">The segment.</param>
	void init(const Segment& s);

	/// <summary>
	/// Links a segment to the current point chain.
	/// </summary>
	/// <param name="s">The segment.</param>
	/// <returns>Returns <c>true</c> if the segment was linked successfully; otherwise, <c>false</c>.</returns>
	bool link_segment(const Segment& s);

	/// <summary>
	/// Links another point chain to the current one.
	/// </summary>
	/// <param name="chain">The chain to link.</param>
	bool link_pointchain(PointChain& chain);

	/// <summary>
	/// Returns whether the point chain is closed.
	/// </summary>
	/// <returns>Returns <c>true</c> if the point chain is closed; otherwise, <c>false</c>.</returns>
	bool closed() const { return _closed; }

	/// <summary>
	/// Gets an iterator that points to the first element in the point chain.
	/// </summary>
	/// <returns></returns>
	iterator begin() { return _l.begin(); }

	/// <summary>
	/// Gets an iterator that points to the last element in the point chain.
	/// </summary>
	/// <returns></returns>
	iterator end() { return _l.end(); }

	/// <summary>
	/// Clears the point chain.
	/// </summary>
	void clear() { _l.clear(); }

	/// <summary>
	/// Gets the size of the point chain.
	/// </summary>
	/// <returns></returns>
	size_t size() const { return _l.size(); }

private:
	list<Point> _l;
	bool _closed; // If true, the chain is a closed chain
};

class Connector
{
public:
	typedef list<PointChain>::iterator iterator;

	/// <summary>
	/// Constructor.
	/// </summary>
	Connector()
		: _open_polygons(), _closed_polygons()
	{
	}

	/// <summary>
	/// Adds a segment to the connector.
	/// </summary>
	/// <param name="s">The segment.</param>
	void add(const Segment& s);

	/// <summary>
	/// Gets an iterator that points to the first closed polygon.
	/// </summary>
	/// <returns>The iterator.</returns>
	iterator begin() { return _closed_polygons.begin(); }

	/// <summary>
	/// Gets an iterator that points to the last closed polygon.
	/// </summary>
	/// <returns>The iterator.</returns>
	iterator end() { return _closed_polygons.end(); }

	/// <summary>
	/// Clears the connector.
	/// </summary>
	void clear()
	{
		_closed_polygons.clear();
		_open_polygons.clear();
	}

	/// <summary>
	/// Gets the number of closed polygons in the connector.
	/// </summary>
	/// <returns>The number of closed polygons.</returns>
	size_t size() const { return _closed_polygons.size(); }

	/// <summary>
	/// Gets the connector contents as a polygon.
	/// </summary>
	/// <param name="p">The result polygon.</param>
	void to_polygon(Polygon& p);

private:
	list<PointChain> _open_polygons;
	list<PointChain> _closed_polygons;

};