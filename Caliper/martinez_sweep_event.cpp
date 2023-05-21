
#include "martinez_sweep_event.h"

namespace martinez
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
	inline bool positive_area(const Point& p0, const Point& p1, const Point& p2)
	{
		return (p0.x - p2.x) * (p1.y - p2.y) < (p1.x - p2.x) * (p0.y - p2.y);
	}

	bool SweepEvent::above(const Point& x)
	{
		if (is_left)
			return positive_area(p, other->p, x);
		else
			return positive_area(other->p, p, x);
	}

	bool SweepEvent::below(const Point& x)
	{
		return !above(x);
	}

	bool SweepEventComparer::operator() (SweepEvent* lhs, SweepEvent* rhs) const
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

	bool SegmentComparer::operator() (SweepEvent* e1, SweepEvent* e2) const
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
}