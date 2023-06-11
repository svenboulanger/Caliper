#include "sweep_martinez.h"
#include "connector.h"

namespace martinez
{
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
		set<SweepEvent*, SegmentComparer> status_line;
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
					e->data.inside = e->data.in_out = false;
				}
				else if ((*prev)->data.edge_type != Normal)
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
						e->data.inside = true;

						// There's nothing before it, so it can only be outside->data.inside
						e->data.in_out = false;
					}
					else
					{
						// The last two edges are overlapping edges
						// We don't know how they're ordered though...
						sli = prev;
						sli--;
						if ((*prev)->data.polygon_type == e->data.polygon_type) {
							e->data.in_out = !(*prev)->data.in_out;
							e->data.inside = !(*sli)->data.in_out;
						}
						else
						{
							e->data.in_out = !(*sli)->data.in_out;
							e->data.inside = !(*prev)->data.in_out;
						}
					}
				}
				else if (e->data.polygon_type == (*prev)->data.polygon_type)
				{
					// This is a normal edge from the same polygon that is not at the start of
					// the status line
					e->data.inside = (*prev)->data.inside;
					e->data.in_out = !(*prev)->data.in_out;
				}
				else
				{
					// This is a normal edge from a different polygon that is not at the start of
					// status line
					e->data.inside = !(*prev)->data.in_out;
					e->data.in_out = (*prev)->data.inside;
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
				switch (e->data.edge_type)
				{
				case Normal:
					switch (op)
					{
					case Intersection:
						if (e->other->data.inside)
							connector.add(e->segment());
						break;

					case Union:
						if (!e->other->data.inside)
							connector.add(e->segment());
						break;

					case Difference:
						if (((e->data.polygon_type == Subject && !e->other->data.inside) ||
							(e->data.polygon_type == Clipping) && e->other->data.inside))
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
		SweepEvent* e1 = store_sweep_event(SweepEvent(s.begin(), true, NULL, SweepEventData(pl)));
		SweepEvent* e2 = store_sweep_event(SweepEvent(s.end(), true, e1, SweepEventData(pl)));
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
		SweepEvent* r = store_sweep_event(SweepEvent(p, false, e, SweepEventData(e->data.polygon_type, e->data.edge_type)));
		SweepEvent* l = store_sweep_event(SweepEvent(p, true, e->other, SweepEventData(e->data.polygon_type, e->data.edge_type)));

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
			e1->data.edge_type = e1->other->data.edge_type = NonContributing;
			e2->data.edge_type = e2->other->data.edge_type = (e1->data.in_out == e2->data.in_out) ? SameTransition : DifferentTransition;
		}
		else if (sorted_events.size() == 3)
		{
			// This is only possible if one of the two points of the segments are the same
			sorted_events[1]->data.edge_type = sorted_events[1]->other->data.edge_type = NonContributing;
			if (sorted_events[0])
			{
				// It's the end points that were shared
				sorted_events[0]->other->data.edge_type = (e1->data.in_out == e2->data.in_out) ? SameTransition : DifferentTransition;
			}
			else
			{
				// It's the begin points that were shared
				sorted_events[2]->other->data.edge_type = (e1->data.in_out == e2->data.in_out) ? SameTransition : DifferentTransition;
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
				sorted_events[1]->data.edge_type = sorted_events[1]->other->data.edge_type = NonContributing;
				sorted_events[2]->data.edge_type = (e1->data.in_out == e2->data.in_out) ? SameTransition : DifferentTransition;
				divide_segment(sorted_events[0], sorted_events[1]->p);
				divide_segment(sorted_events[1], sorted_events[2]->p);
				return;
			}
			else
			{
				// One segment completely includes the other one

				// Disable the inner segment, we'll store such information in the other overlapping segment
				sorted_events[1]->data.edge_type = sorted_events[1]->other->data.edge_type = NonContributing;

				// We need to divide the longer segment in 3 parts
				divide_segment(sorted_events[0], sorted_events[1]->p);
				SweepEvent* second_part = sorted_events[3]->other;
				divide_segment(second_part, sorted_events[2]->p);

				// Update the overlapping segment to have a special status
				second_part->data.edge_type = second_part->other->data.edge_type = (e1->data.in_out == e2->data.in_out) ? SameTransition : DifferentTransition;
			}
		}
	}
}