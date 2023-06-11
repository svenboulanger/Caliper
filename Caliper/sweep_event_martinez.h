#pragma once

#include "sweep_event.h"
#include <set>

namespace martinez
{
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

	/// <summary>
	/// The sweep event data necessary for the Martinez et al. algorithm.
	/// </summary>
	struct SweepEventData
	{
		/// <summary>
		/// The polygon that the event belongs to.
		/// </summary>
		PolygonType polygon_type;

		/// <summary>
		/// Describes the type of edge.
		/// </summary>
		EdgeType edge_type;

		/// <summary>
		/// Describes whether the edge is one going from inside the polygon to outside the polygon
		/// </summary>
		bool in_out;

		/// <summary>
		/// Describes whether we are inside an outer polygon.
		/// </summary>
		bool inside;

		/// <summary>
		/// Constructor.
		/// </summary>
		SweepEventData()
			: polygon_type(), edge_type(), in_out(), inside()
		{
		}

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="e_polygon_type">The polygon type the event belongs to.</param>
		/// <param name="e_edge_type">The edge type.</param>
		SweepEventData(const PolygonType e_polygon_type, const EdgeType e_edge_type = Normal)
			: polygon_type(e_polygon_type), edge_type(e_edge_type), in_out(false), inside(false)
		{
		}
	};

	typedef sweep_line::SweepEvent<SweepEventData> SweepEvent;
	typedef sweep_line::SweepEventComparer<SweepEventData> SweepEventComparer;
	typedef sweep_line::SegmentComparer<SweepEventData> SegmentComparer;
}