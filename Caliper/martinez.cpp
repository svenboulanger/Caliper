#include "martinez.h"
#include "connector.h"

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
inline bool are_collinear(const Point& p0, const Point& p1, const Point& p2)
{
	return (p0.x - p2.x) * (p1.y - p2.y) == (p1.x - p2.x) * (p0.y - p2.y);
}
inline int merge_sort_intersection(const long x0, const long x1, const long x2, const long x3, const Segment& seg0, const Segment& seg1, Point& pi0, Point& pi1)
{
	// This code was generated using a script to avoid mistakes
	if (x0 < x1)
	{
		if (x2 < x3)
		{
			if (x0 < x2)
			{
				if (x1 < x2)
				{
					// [x0, x1, x2, x3]
					if (x1 == x2)
					{
						pi0 = seg0.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x1 < x3)
					{
						// [x0, x2, x1, x3]
						pi0 = seg1.begin();
						if (x2 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
					else
					{
						// [x0, x2, x3, x1]
						pi0 = seg1.begin();
						if (x2 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x3 < x0)
				{
					// [x2, x3, x0, x1]
					if (x3 == x0)
					{
						pi0 = seg1.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x3 < x1)
					{
						// [x2, x0, x3, x1]
						pi0 = seg0.begin();
						if (x0 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
					else
					{
						// [x2, x0, x1, x3]
						pi0 = seg0.begin();
						if (x0 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
				}
			}
		}
		else
		{
			if (x0 < x3)
			{
				if (x1 < x3)
				{
					// [x0, x1, x3, x2]
					if (x1 == x3)
					{
						pi0 = seg0.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x1 < x2)
					{
						// [x0, x3, x1, x2]
						pi0 = seg1.end();
						if (x3 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
					else
					{
						// [x0, x3, x2, x1]
						pi0 = seg1.end();
						if (x3 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x2 < x0)
				{
					// [x3, x2, x0, x1]
					if (x2 == x0)
					{
						pi0 = seg1.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x2 < x1)
					{
						// [x3, x0, x2, x1]
						pi0 = seg0.begin();
						if (x0 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
					else
					{
						// [x3, x0, x1, x2]
						pi0 = seg0.begin();
						if (x0 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
				}
			}
		}
	}
	else
	{
		if (x2 < x3)
		{
			if (x1 < x2)
			{
				if (x0 < x2)
				{
					// [x1, x0, x2, x3]
					if (x0 == x2)
					{
						pi0 = seg0.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x0 < x3)
					{
						// [x1, x2, x0, x3]
						pi0 = seg1.begin();
						if (x2 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
					else
					{
						// [x1, x2, x3, x0]
						pi0 = seg1.begin();
						if (x2 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x3 < x1)
				{
					// [x2, x3, x1, x0]
					if (x3 == x1)
					{
						pi0 = seg1.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x3 < x0)
					{
						// [x2, x1, x3, x0]
						pi0 = seg0.end();
						if (x1 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
					else
					{
						// [x2, x1, x0, x3]
						pi0 = seg0.end();
						if (x1 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
				}
			}
		}
		else
		{
			if (x1 < x3)
			{
				if (x0 < x3)
				{
					// [x1, x0, x3, x2]
					if (x0 == x3)
					{
						pi0 = seg0.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x0 < x2)
					{
						// [x1, x3, x0, x2]
						pi0 = seg1.end();
						if (x3 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
					else
					{
						// [x1, x3, x2, x0]
						pi0 = seg1.end();
						if (x3 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x2 < x1)
				{
					// [x3, x2, x1, x0]
					if (x2 == x1)
					{
						pi0 = seg1.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x2 < x0)
					{
						// [x3, x1, x2, x0]
						pi0 = seg0.end();
						if (x1 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
					else
					{
						// [x3, x1, x0, x2]
						pi0 = seg0.end();
						if (x1 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
				}
			}
		}
	}
}

int find_intersection(const Segment& seg0, const Segment& seg1, Point& pi0, Point& pi1)
{
	const Point& p0 = seg0.begin();
	const Point& p1 = seg1.begin();
	Point d0 = seg0.end() - p0;
	Point d1 = seg1.end() - p1;
	Point e = p1 - p0;

	long m = d0.x * d1.y - d0.y * d1.x;
	if (m == 0)
	{
		// The two lines are parallel
		if (!are_collinear(p0, seg0.end(), p1))
		{
			// The two lines are not collinear
			return 0;
		}

		// The two segments are on one line, go over the different options
		if (d0.x != 0)
		{
			// The segments are not horizontal, so we can simply take a look at the point coordinates
			return merge_sort_intersection(seg0.p1.x, seg0.p2.x, seg1.p1.x, seg1.p2.x, seg0, seg1, pi0, pi1);
		}
		else
		{
			// The segment was horizontal, let's use the X-coordinates instead
			return merge_sort_intersection(seg0.p1.y, seg0.p2.y, seg1.p1.y, seg1.p2.y, seg0, seg1, pi0, pi1);
		}
	}
	else
	{
		Point e = seg1.p1 - seg0.p1;
		long n1 = e.x * d1.y - d1.x * e.y;
		long n2 = e.x * d0.y - d0.x * e.y;
		if (m < 0)
		{
			if (n1 > 0 || n2 > 0 || n1 < m || n2 < m)
				return 0;
			pi0 = Point(seg0.p1.x + (n1 * d0.x) / m, seg0.p1.y + (n1 * d0.y) / m);
		}
		else
		{
			if (n1 < 0 || n2 < 0 || n1 > m || n2 > m)
				return 0;
			pi0 = Point(seg0.p1.x + (n1 * d0.x) / m, seg0.p1.y + (n1 * d0.y) / m);
		}
		return 1;
	}
}

bool BooleanOperation::SweepEvent::above(const Point& x)
{
	if (is_left)
		return positive_area(p, other->p, x);
	else
		return positive_area(other->p, p, x);
}

bool BooleanOperation::SweepEvent::below(const Point& x)
{
	return !above(x);
}

bool BooleanOperation::SweepEventComparer::operator() (SweepEvent* lhs, SweepEvent* rhs) const
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

bool BooleanOperation::SegmentComparer::operator() (SweepEvent* e1, SweepEvent* e2) const
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

void BooleanOperation::compute(BooleanOperationType op, Polygon& result)
{
	// Test 1 for trivial result case
	if (subject.contour_count() * clipping.contour_count() == 0)
	{
		// At least one of the polygons is empty
		if (op == Difference)
			result = subject;
		else if (op == Union || op == Xor)
			result = (subject.contour_count() == 0) ? clipping : subject;
		return;
	}

	// Test 2 for trivial result case
	Point minsubj, maxsubj, minclip, maxclip;
	subject.bounding_box(minsubj, maxsubj);
	clipping.bounding_box(minclip, maxclip);
	if (minsubj.x > maxclip.x || minclip.x > maxsubj.x || minsubj.y > maxclip.y || minclip.y > maxsubj.y)
	{
		// The bounding boxes do not overlap
		if (op == Difference)
			result = subject;
		if (op == Union || op == Xor) {
			result = subject;
			for (unsigned int i = 0; i < clipping.contour_count(); i++) {
				result.push_back(Contour());
				result.back() = clipping.contour(i);
			}
		}
		return;
	}

	// Non-trivial boolean operation

	// Step 1: insert all endpoints associated to the line segments into the event queue
	for (unsigned int i = 0; i < subject.contour_count(); i++)
		for (unsigned int j = 0; j < subject.contour(i).vertex_count(); j++)
			process_segment(subject.contour(i).segment(j), Subject);
	for (unsigned i = 0; i < clipping.contour_count(); i++)
		for (unsigned j = 0; j < clipping.contour(i).vertex_count(); j++)
			process_segment(clipping.contour(i).segment(j), Clipping);

	Connector connector;
	set<SweepEvent*, SegmentComparer> status_line; // Status line
	set<SweepEvent*, SegmentComparer>::iterator it, sli, prev, next;
	SweepEvent* e;

	while (!eq.empty())
	{
		e = eq.top();
		eq.pop();

		if (e->is_left)
		{
			// The event points to the start of a segment
			// We will have to insert it in the status line and compare it 
			// to its neighbors to find intersection points

			// Insert into the status line and find the previous item
			e->it = it = status_line.insert(e).first;

			next = prev = it;
			if (prev != status_line.begin())
				prev--;
			else
				prev = status_line.end();

			// Compute the inside and in_out flags
			if (prev == status_line.end())
			{
				// There is no previous segment, so that means the segment
				// should mark an edge that goes from the outside to the inside
				e->inside = e->in_out = false;
			}
			else if ((*prev)->edge_type != Normal)
			{
				// This only happens if the previous edge is an overlapping edge.

				// Note that this part can also happen if the second overlapping edge is added
				// to the status line. In this case, it is possible that the prev segment
				// is just the one that was made non-contributing, but there's none before that one.
				if (prev == status_line.begin())
				{
					// The current segment is the overlapping segment
					// Since the inside parameter is never going to be used, it's ok to put
					// whatever for it
					e->inside = true;

					// There's nothing before it, so it can only be outside->inside
					e->in_out = false;
				}
				else
				{
					// The last two edges are overlapping edges
					// We don't know how they're ordered though...
					sli = prev;
					sli--;
					if ((*prev)->polygon_type == e->polygon_type) {
						e->in_out = !(*prev)->in_out;
						e->inside = !(*sli)->in_out;
					}
					else
					{
						e->in_out = !(*sli)->in_out;
						e->inside = !(*prev)->in_out;
					}
				}
			}
			else if (e->polygon_type == (*prev)->polygon_type)
			{
				// This is a normal edge from the same polygon that is not at the start of
				// the status line
				e->inside = (*prev)->inside;
				e->in_out = !(*prev)->in_out;
			}
			else
			{
				// This is a normal edge from a different polygon that is not at the start of
				// status line
				e->inside = !(*prev)->in_out;
				e->in_out = (*prev)->inside;
			}

			// Process a possible intersection between e and its neighbors
			if ((++next) != status_line.end())
				possible_intersection(e, *next);
			if (prev != status_line.end())
				possible_intersection(*prev, e);
		}
		else
		{
			// The event points to the end of a segment
			// We will have to remove the event and have the neightbors check
			// with each other for intersections
			next = prev = sli = e->other->it;

			// Get the next and previous line segments to the event in the status line
			++next;
			if (prev != status_line.begin())
				prev--;
			else
				prev = status_line.end();

			// Check if the line segment belongs to the boolean operation result and
			// add the segment to the connector if it should be part of the result
			switch (e->edge_type)
			{
			case Normal:
				switch (op)
				{
				case Intersection:
					if (e->other->inside)
						connector.add(e->segment());
					break;

				case Union:
					if (!e->other->inside)
						connector.add(e->segment());
					break;

				case Difference:
					if (((e->polygon_type == Subject && !e->other->inside) ||
						(e->polygon_type == Clipping) && e->other->inside))
						connector.add(e->segment());
					break;

				case Xor:
					connector.add(e->segment());
				}
				break;

			case SameTransition:
				if (op == Intersection || op == Union)
					connector.add(e->segment());
				break;

			case DifferentTransition:
				if (op == Difference)
					connector.add(e->segment());
				break;
			}

			// Delete line segment associated to e from S and check the previous/next intersections
			status_line.erase(sli);
			if (next != status_line.end() && prev != status_line.end())
				possible_intersection(*prev, *next);
		}
	}

	connector.to_polygon(result);
}

void BooleanOperation::process_segment(const Segment& s, PolygonType pl)
{
	if (s.begin() == s.end())
		return;

	// Create the two events
	SweepEvent* e1 = store_sweep_event(SweepEvent(s.begin(), true, pl, NULL));
	SweepEvent* e2 = store_sweep_event(SweepEvent(s.end(), true, pl, e1));
	e1->other = e2;

	// Assign which of the events is the left and which one is the right
	if (e1->p.x < e2->p.x)
		e2->is_left = false;
	else if (e1->p.x > e2->p.x)
		e1->is_left = false;
	else if (e1->p.y < e2->p.y)
		e2->is_left = false;
	else
		e1->is_left = false;

	// Push on the priority queue
	eq.push(e1);
	eq.push(e2);
}

void BooleanOperation::divide_segment(SweepEvent* e, const Point& p)
{
	// Create the two new events
	SweepEvent* r = store_sweep_event(SweepEvent(p, false, e->polygon_type, e, e->edge_type));
	SweepEvent* l = store_sweep_event(SweepEvent(p, true, e->polygon_type, e->other, e->edge_type));

	// Fix the pointers
	e->other->other = l;
	e->other = r;
	
	// Push the new segments on the priority queue
	eq.push(l);
	eq.push(r);
}

void BooleanOperation::possible_intersection(SweepEvent* e1, SweepEvent* e2)
{
	// Check the intersection
	Point ip1, ip2;
	int intersections;

	if (!(intersections = find_intersection(e1->segment(), e2->segment(), ip1, ip2)))
		// No intersections
		return;

	if (intersections == 1)
	{
		// Single intersection
		if (e1->p == e2->p || e1->other->p == e2->other->p)
		{
			// The intersections are at an endpoint of both line segments,
			// which means we're not really interested in them...
			return;
		}
		else
		{
			if (e1->p != ip1 && e1->other->p != ip1)
				// ip1 is not an endpoint of the first segment
				divide_segment(e1, ip1);
			if (e2->p != ip1 && e2->other->p != ip1)
				// ip1 is not an endpoint of the second segment
				divide_segment(e2, ip1);
			return;
		}
	}

	// There is a whole overlapping segment...
	// Let's build a list of events that are sorted accordingly
	vector<SweepEvent*> sorted_events;

	// Sort the two points of the events
	if (e1->p == e2->p)
		sorted_events.push_back(NULL);
	else if (sec(e1, e2))
	{
		sorted_events.push_back(e2);
		sorted_events.push_back(e1);
	}
	else
	{
		sorted_events.push_back(e1);
		sorted_events.push_back(e2);
	}

	// Sort the two other ends of the events
	if (e1->other->p == e2->other->p)
		sorted_events.push_back(NULL);
	else if (sec(e1->other, e2->other))
	{
		sorted_events.push_back(e2->other);
		sorted_events.push_back(e1->other);
	}
	else
	{
		sorted_events.push_back(e1->other);
		sorted_events.push_back(e2->other);
	}

	if (sorted_events.size() == 2)
	{
		// This is only possible if a NULL has been pushed on sorted_events
		// Meaning that both edges are identical
		e1->edge_type = e1->other->edge_type = NonContributing;
		e2->edge_type = e2->other->edge_type = (e1->in_out == e2->in_out) ? SameTransition : DifferentTransition;
	}
	else if (sorted_events.size() == 3)
	{
		// This is only possible if one of the two points of the segments are the same
		sorted_events[1]->edge_type = sorted_events[1]->other->edge_type = NonContributing;
		if (sorted_events[0])
		{
			// It's the end points that were shared
			sorted_events[0]->other->edge_type = (e1->in_out == e2->in_out) ? SameTransition : DifferentTransition;
		}
		else
		{
			// It's the begin points that were shared
			sorted_events[2]->other->edge_type = (e1->in_out == e2->in_out) ? SameTransition : DifferentTransition;
		}
		divide_segment(sorted_events[0] ? sorted_events[0] : sorted_events[2]->other, sorted_events[1]->p);
	}
	else
	{
		// General case, there is a whole segment of overlap given by the middle 2 elements of sorted_events
		if (sorted_events[0] != sorted_events[3]->other)
		{
			// There's a common segment, but also two separate ones

			// Disable the start point of the second segment
			sorted_events[1]->edge_type = sorted_events[1]->other->edge_type = NonContributing;
			sorted_events[2]->edge_type = (e1->in_out == e2->in_out) ? SameTransition : DifferentTransition;
			divide_segment(sorted_events[0], sorted_events[1]->p);
			divide_segment(sorted_events[1], sorted_events[2]->p);
			return;
		}
		else
		{
			// One segment completely includes the other one

			// Disable the inner segment, we'll store such information in the other overlapping segment
			sorted_events[1]->edge_type = sorted_events[1]->other->edge_type = NonContributing;

			// We need to divide the longer segment in 3 parts
			divide_segment(sorted_events[0], sorted_events[1]->p);
			SweepEvent* second_part = sorted_events[3]->other;
			divide_segment(second_part, sorted_events[2]->p);

			// Update the overlapping segment to have a special status
			second_part->edge_type = second_part->other->edge_type = (e1->in_out == e2->in_out) ? SameTransition : DifferentTransition;
		}
	}
}