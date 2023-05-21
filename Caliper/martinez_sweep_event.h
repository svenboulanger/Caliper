#pragma once

#include "point.h"
#include "segment.h"
#include <set>

namespace martinez
{
	inline long signed_area(const Point& p0, const Point& p1, const Point& p2);
	inline bool positive_area(const Point& p0, const Point& p1, const Point& p2);

	struct SweepEvent;
	enum EdgeType
	{
		/// <summary>
		/// Indicates a normal edge.
		/// </summary>
		Normal,

		/// <summary>
		/// This flag indicates that the edge shouldn't be considered anymore.
		/// </summary>
		NonContributing,

		/// <summary>
		/// Indicates that the edge is actually two edges on top of each other with the same transition from inside to outside.
		/// </summary>
		SameTransition,

		/// <summary>
		/// Indicates that the edge is actually two edges on top of each other with a different transition from inside to outside.
		/// </summary>
		DifferentTransition
	};
	enum PolygonType
	{
		Subject,
		Clipping
	};
	struct SweepEventComparer {
		bool operator() (SweepEvent* e1, SweepEvent* e2) const;
	};
	struct SegmentComparer {
		bool operator() (SweepEvent* e1, SweepEvent* e2) const;
	};

	struct SweepEvent {

		/// <summary>
		/// The point for the event.
		/// </summary>
		Point p;

		/// <summary>
		/// If <c>true</c>, then the event marks the start of a new segment.
		/// </summary>
		bool is_left;

		/// <summary>
		/// The polygon that the event belongs to.
		/// </summary>
		PolygonType polygon_type;

		/// <summary>
		/// Describes the type of edge.
		/// </summary>
		EdgeType edge_type;

		/// <summary>
		/// The event associated to the other point of the segment.
		/// </summary>
		SweepEvent* other;

		/// <summary>
		/// Describes whether the edge is one going from inside the polygon to outside the polygon
		/// </summary>
		bool in_out;

		/// <summary>
		/// Describes whether we are inside an outer polygon.
		/// </summary>
		bool inside;

		/// <summary>
		/// The iterator that points to this event in the status line.
		/// </summary>
		set<SweepEvent*, SegmentComparer>::iterator it;

		/// <summary>
		/// Constructor.
		/// </summary>
		SweepEvent()
			: p(), is_left(false), in_out(false), inside(false), other(NULL), edge_type(Normal), polygon_type(Subject), it()
		{
		}

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="pp">The point.</param>
		/// <param name="b_is_left">If <c>true</c>, the event indicates the start point.</param>
		/// <param name="b_polygon_type">The polygon that the event belongs to.</param>
		/// <param name="o">The other sweep event for the same segment.</param>
		/// <param name="b_edge_type">The type of edge.</param>
		SweepEvent(const Point& pp, bool b_is_left, PolygonType b_polygon_type, SweepEvent* o, EdgeType b_edge_type = Normal)
			: p(pp), is_left(b_is_left), in_out(false), inside(false), other(o), edge_type(b_edge_type), polygon_type(b_polygon_type), it()
		{
		}

		/// <summary>
		/// Gets the sweep event represented as a segment.
		/// </summary>
		/// <returns>The segment.</returns>
		const Segment segment() const { return Segment(p, other->p); }

		/// <summary>
		/// Determines whether the segment is below the point x.
		/// </summary>
		/// <param name="x">The point.</param>
		/// <returns>Returns <c>true</c> if the specified point is below the segment; otherwise, <c>false</c>.</returns>
		bool below(const Point& x);

		/// <summary>
		/// Determines whether the segment is above the point x.
		/// </summary>
		/// <param name="x">The point.</param>
		/// <returns>Returns <c>true</c> if the specified point is above the segment; otherwise, <c>false</c>.</returns>
		bool above(const Point& x);
	};
}