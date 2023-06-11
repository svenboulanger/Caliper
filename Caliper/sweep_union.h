#pragma once

#include "polygon.h"
#include "sweep_event_union.h"
#include <queue>
#include <vector>

namespace sweep_union
{
	struct PolygonUnion
	{
		/// <summary>
		/// Performs the union operation.
		/// </summary>
		/// <param name="result">The polygon that will contain the result.</param>
		void compute(Polygon& result);

		/// <summary>
		/// Adds a polygon to the operation.
		/// </summary>
		/// <param name="polygon">The polygon.</param>
		void add(Polygon& polygon);

	private:
		priority_queue<SweepEvent*, vector<SweepEvent*>, SweepEventComparer> eq;
		set<SweepEvent*, SegmentComparer> status_line;
		deque<SweepEvent> event_holder;
		SweepEventComparer sec;
		
		void process_segment(const Segment& s);
		bool handle_intersections(SweepEvent* prev, SweepEvent* next);
		inline void merge_overlapping_events(SweepEvent* prev, SweepEvent* next, bool prev_in_status_line = true);
		SweepEvent* divide_segment(SweepEvent* e, const Point& p);
		SweepEvent* store_sweep_event(const SweepEvent& e)
		{
			event_holder.push_back(e);
			return &event_holder.back();
		}
	};
}