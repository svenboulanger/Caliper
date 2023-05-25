#pragma once

#include "point.h"
#include "segment.h"
#include <set>

namespace sweep_line
{
	/// <summary>
	/// Computes the signed area of a trapezoid formed by the points. Can be used to determine the orientation.
	/// </summary>
	/// <param name="p0">The orientation point.</param>
	/// <param name="p1">The first point.</param>
	/// <param name="p2">The second point.</param>
	/// <returns>A value that is positive is going from p1 to p2 around p0 is clockwise, negative if counter-clockwise, or 0 if collinear.</returns>
	inline long signed_area(const Point& p0, const Point& p1, const Point& p2)
	{
		return (p2.x - p0.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p0.y);
	}

	/// <summary>
	/// Computes wether p1 turns clockwise around p0 to reach p2.
	/// </summary>
	/// <param name="p0">The reference point.</param>
	/// <param name="p1">The point.</param>
	/// <param name="p2">The target point.</param>
	/// <returns>Returns <c>true</c> if we need to turn clockwise to get from p1 to p2 around p0; otherwise, <c>false</c>.</returns>
	inline bool turns_clockwise(const Point& p0, const Point& p1, const Point& p2)
	{
		return (p2.x - p0.x) * (p1.y - p0.y) > (p1.x - p0.x) * (p2.y - p0.y);
	}

	template<class T>
	struct SweepEvent;

	/// <summary>
	/// A comparer for sweep events when they're added to the priority queue.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	struct SweepEventComparer {
		bool operator() (SweepEvent<T>* lhs, SweepEvent<T>* rhs) const
		{
			// Note: because this comparer is used in a priority queue which returns the "largest" element
			// first, we will order in the opposite direction...

			// First priority is ordering by X-coordinates
			if (lhs->p.x < rhs->p.x)
				return false;
			if (rhs->p.x < lhs->p.x)
				return true;

			// Second priority is ordering by Y-coordinates
			if (lhs->p.y < rhs->p.y)
				return false;
			if (rhs->p.y < lhs->p.y)
				return true;

			// Both points are equal, give priority to the right endpoints
			if (lhs->is_left != rhs->is_left)
				return lhs->is_left;

			// No distinction between the two starting points...
			// We would like to use the bottom one of the two
			return lhs->above(rhs->other->p);
		}
	};

	/// <summary>
	/// A comparer for sweep events when they're added to the status line.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	struct SegmentComparer {
		bool operator() (SweepEvent<T>* e1, SweepEvent<T>* e2) const
		{
			// Note: only start events are stored in the status line, so we don't need to check for that anymore
			if (e1 == e2)
				return false;

			if (e1->p.x == e2->p.x)
			{
				// Both segments start at the same x-coordinate, use the y-coordinates instead
				if (e1->p.y == e2->p.y)
				{
					// Both start points are identical, use the other segments to compare
					long orientation = signed_area(e1->p, e1->other->p, e2->other->p);
					if (orientation < 0)
						return true;
					else if (orientation > 0)
						return false;
					else
						return e1 < e2; // No way to differentiate between the two, just some consistent criterion
				}
				else if (e1->p.y < e2->p.y)
				{
					long orientation = signed_area(e1->p, e1->other->p, e2->p);
					if (orientation < 0)
						return true;
					else if (orientation > 0)
						return false;
					else
						return signed_area(e1->p, e1->other->p, e2->other->p) <= 0; // Return true (e1 first) if collinear
				}
				else
				{
					long orientation = signed_area(e2->p, e2->other->p, e1->p);
					if (orientation > 0)
						return true;
					else if (orientation < 0)
						return false;
					else
						return signed_area(e2->p, e2->other->p, e1->other->p) > 0; // Return false (e1 first) if collinear
				}
			}
			else if (e1->p.x < e2->p.x)
			{
				// e1 comes first, so we will compare the points of e2 to e1
				long orientation = signed_area(e1->p, e1->other->p, e2->p);
				if (orientation < 0)
					return true;
				else if (orientation > 0)
					return false;
				else
					return signed_area(e1->p, e1->other->p, e2->other->p) <= 0; // Return true (e1 first) if collinear.
			}
			else
			{
				// e2 comes first, so we will compare the points of e1 to e2
				long orientation = signed_area(e2->p, e2->other->p, e1->p);
				if (orientation > 0)
					return true;
				else if (orientation < 0)
					return false;
				else
					return signed_area(e2->p, e2->other->p, e1->other->p) > 0; // Return false (e2 first) if collinear.
			}
		}
	};

	template<class T>
	struct SweepEvent
	{
		/// <summary>
		/// The point for the event.
		/// </summary>
		Point p;

		/// <summary>
		/// If <c>true</c>, then the event marks the start of a new segment.
		/// </summary>
		bool is_left;

		/// <summary>
		/// The event associated to the other point of the segment.
		/// </summary>
		SweepEvent* other;

		/// <summary>
		/// The extra data associated to the event.
		/// </summary>
		T data;

		/// <summary>
		/// The iterator that points to this event in the status line.
		/// </summary>
		typename set<SweepEvent<T>*, SegmentComparer<T>>::iterator it;

		/// <summary>
		/// Constructor.
		/// </summary>
		SweepEvent()
			: p(), is_left(false), other(NULL), data()
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
		SweepEvent(const Point& p_p, bool b_is_left, SweepEvent<T>* p_other, T d_data)
			: p(p_p), is_left(b_is_left), other(p_other), data(d_data)
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
		bool below(const Point& x) const { return !above(x); }

		/// <summary>
		/// Determines whether the segment is above the point x.
		/// </summary>
		/// <param name="x">The point.</param>
		/// <returns>Returns <c>true</c> if the specified point is above the segment; otherwise, <c>false</c>.</returns>
		bool above(const Point& x) const
		{
			if (is_left)
				return turns_clockwise(p, other->p, x);
			else
				return turns_clockwise(other->p, p, x);
		}
	};
}