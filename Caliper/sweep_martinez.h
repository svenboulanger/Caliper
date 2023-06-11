#pragma once

#include "polygon.h"
#include <set>
#include <queue>
#include <vector>
#include "sweep_event_martinez.h"

namespace martinez
{
	class BooleanOperation {
	public:
		/// <summary>
		/// The type of boolean operation.
		/// </summary>
		enum BooleanOperationType
		{
			/// <summary>
			/// Computes the intersection of the polygons.
			/// </summary>
			Intersection,

			/// <summary>
			/// Computes the union of the polygons.
			/// </summary>
			Union,

			/// <summary>
			/// Computes the difference of the polygons.
			/// </summary>
			Difference,

			/// <summary>
			/// Computes the XOR operation result of the polygons.
			/// </summary>
			Xor
		};

		/// <summary>
		/// Constructor.
		/// </summary>
		BooleanOperation(Polygon& p_subject, Polygon& p_clipping)
			: eq(), event_holder(), subject(p_subject), clipping(p_clipping), sec()
		{
		}

		/// <summary>
		/// Performs the boolean operation.
		/// </summary>
		/// <param name="op">The operator.</param>
		/// <param name="result">The polygon that will contain the result.</param>
		void compute(BooleanOperationType op, Polygon& result);

	private:
		priority_queue<SweepEvent*, vector<SweepEvent*>, SweepEventComparer> eq;
		deque<SweepEvent> event_holder;
		Polygon& subject;
		Polygon& clipping;
		const SweepEventComparer sec;
		void process_segment(const Segment& s, PolygonType polygon_type);
		void possible_intersection(SweepEvent* e1, SweepEvent* e2);
		void divide_segment(SweepEvent* e, const Point& p);
		SweepEvent* store_sweep_event(const SweepEvent& e) {
			event_holder.push_back(e);
			return &event_holder.back();
		}
	};
}