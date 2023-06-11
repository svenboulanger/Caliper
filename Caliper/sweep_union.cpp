#include "sweep_event_union.h"
#include "sweep_union.h"
#include "connector.h"

namespace sweep_union
{
	void PolygonUnion::add(Polygon& polygon)
	{
		for (unsigned int i = 0; i < polygon.contour_count(); i++)
		{
			for (unsigned int j = 0; j < polygon.contour(i).vertex_count(); j++)
			{
				process_segment(polygon.contour(i).segment(j));
			}
		}
	}

	void PolygonUnion::process_segment(const Segment& segment)
	{
		if (segment.begin() == segment.end())
			return; // Trivial edge, will not do anything

		// Create the two events
		SweepEvent* e1 = store_sweep_event(SweepEvent(segment.begin(), true, NULL, SweepEventData()));
		SweepEvent* e2 = store_sweep_event(SweepEvent(segment.end(), true, e1, SweepEventData()));
		e1->other = e2;

		// One of the two events will be considered the left event
		if (e1->p.x < e2->p.x)
		{
			e2->is_left = false;
			e1->data.out_ins = e2->data.out_ins = 1;
		}
		else if (e2->p.x < e1->p.x)
		{
			e1->is_left = false;
			e1->data.out_ins = e2->data.out_ins = -1;
		}
		else if (e1->p.y < e2->p.y)
		{
			e2->is_left = false;
			e1->data.out_ins = e2->data.out_ins = 1;
		}
		else
		{
			// Note that equality between points was already tested,
			// so this part is only if e1->p.y > e2->p.y
			e1->is_left = false;
			e1->data.out_ins = e2->data.out_ins = -1;
		}

		// Push on the priority queue
		eq.push(e1);
		eq.push(e2);
	}

	void PolygonUnion::compute(Polygon& result)
	{
		Connector connector;
		set<SweepEvent*, SegmentComparer>::iterator it, prev, next;
		SweepEvent* e;
		status_line.clear();

		while (!eq.empty())
		{
			e = eq.top();
			eq.pop();
			if (e->data.overlapped)
				continue;

			if (e->is_left)
			{
				// Start of a segment reached, insert it in the status line
				e->it = it = status_line.insert(e).first;

				// Get the neighboring segments
				next = prev = it;
				++next;
				if (prev != status_line.begin())
					prev--;
				else
					prev = status_line.end();

				if (prev != status_line.end())
					e->data.out_ins_before = (*prev)->data.out_ins_before + (*prev)->data.out_ins;
				else
					e->data.out_ins_before = 0;

				// Process possible intersections
				bool is_overlapping = false;
				if (prev != status_line.end())
					is_overlapping |= handle_intersections(*prev, e);
				if (!is_overlapping && next != status_line.end())
					is_overlapping |= handle_intersections(e, *next);
			}
			else
			{
				// The segment marks the end of a segment
				next = prev = it = e->other->it;

				// Get the previous and next item
				++next;
				if (prev != status_line.begin())
					prev--;
				else
					prev = status_line.end();

				// Only consider non-overlapped events that also our marking a transition
				if (e->other->data.out_ins != 0)
				{
					if (e->other->data.out_ins > 0)
					{
						// If we are going inside any number of polygons, that means the edge needs to be added
						if (e->other->data.out_ins_before == 0)
							connector.add(e->segment());
					}
					else
					{
						// We are going from outside to inside a number of polygons
						// Only add the edge if we are ending up outside
						if (e->other->data.out_ins_before + e->other->data.out_ins == 0)
							connector.add(e->segment());
					}
				}

				// Delete the line segment
				status_line.erase(it);
				if (next != status_line.end() && prev != status_line.end())
					handle_intersections(*prev, *next);
			}
		}

		connector.to_polygon(result);
	}

	/// <summary>
	/// Merges two sweep events. The next event is kept.
	/// </summary>
	inline void PolygonUnion::merge_overlapping_events(SweepEvent* prev, SweepEvent* next, bool prev_in_status_line)
	{
		// The edge becomes the combined effect
		next->data.out_ins += prev->data.out_ins;

		// Since it is overlapping, we should not be using the information from the edge before it
		next->data.out_ins_before = prev->data.out_ins_before;

		// Remove the overlapped edge from the status line
		if (prev_in_status_line)
			status_line.erase(prev->it);
		prev->data.overlapped = true;
		prev->other->data.overlapped = true;
	}

	bool PolygonUnion::handle_intersections(SweepEvent* prev, SweepEvent* next)
	{
		if (prev == next)
			return false; // Both area already the same edge, no need to do anything

		Point ip1, ip2;
		int intersections;

		if (!(intersections = find_intersection(prev->segment(), next->segment(), ip1, ip2)))
		{
			// No intersections, simply copy
			return false;
		}

		if (intersections == 1)
		{
			// Single intersection
			if (prev->p == next->p || prev->other->p == next->other->p)
			{
				// The intersection is at an endpoint, this case we can ignore
				return false;
			}
			else
			{
				if (prev->p != ip1 && prev->other->p != ip1)
				{
					// The point lies somewhere along prev, divide it
					divide_segment(prev, ip1);
				}
				if (next->p != ip1 && next->other->p != ip1)
				{
					// The point lies somewhere along next, divide it
					divide_segment(next, ip1);
				}
				return false;
			}
		}

		// Overlapping segments...
		if (prev->p == next->p)
		{
			// Same starting points
			if (prev->other->p == next->other->p)
			{
				// Both the start and end points are identical, let's merge and delete one of the two
				merge_overlapping_events(prev, next);
			}
			else if (sec(prev->other, next->other))
			{
				// The order is prev.start/next.start - next.end - prev.end
				divide_segment(prev, ip2);
				merge_overlapping_events(prev, next);
			}
			else
			{
				// The order is prev.start/next.start - prev.end - next.end
				divide_segment(next, ip2);
				merge_overlapping_events(prev, next);
			}
		}
		else
		{
			if (prev->other->p == next->other->p)
			{
				// Same end points
				// The order is prev.start - next.start - prev.end/next.end
				SweepEvent* l = divide_segment(prev, ip1);
				merge_overlapping_events(l, next, false);
			}
			else if (sec(prev->other, next->other))
			{
				// The order is prev.start - next.start - next.end - prev.end
				SweepEvent* l = divide_segment(prev, ip1);
				divide_segment(l, ip2);
				merge_overlapping_events(l, next, false);
			}
			else
			{
				// The end point of prev comes before the end point of next
				// There is a partial overlap between the segments
				if (prev->p == ip1)
				{
					// next.start - prev.start - next.end - prev.end
					SweepEvent* l = divide_segment(next, ip1);
					divide_segment(prev, ip2);
					merge_overlapping_events(prev, l);
				}
				else
				{
					// prev.start - next.start - prev.end - next.end
					SweepEvent* l = divide_segment(prev, ip1);
					divide_segment(next, ip2);
					merge_overlapping_events(l, next, false);
				}
			}
		}
		return true;
	}

	SweepEvent* PolygonUnion::divide_segment(SweepEvent* e, const Point& p)
	{
		// Create the two new events
		SweepEvent* r = store_sweep_event(SweepEvent(p, false, e, SweepEventData(e->data.out_ins, e->data.out_ins_before)));
		SweepEvent* l = store_sweep_event(SweepEvent(p, true, e->other, SweepEventData(e->data.out_ins, e->data.out_ins_before)));

		// Fix the pointers
		e->other->other = l;
		e->other = r;

		// Add the new segments
		eq.push(l);
		eq.push(r);
		return l;
	}
}