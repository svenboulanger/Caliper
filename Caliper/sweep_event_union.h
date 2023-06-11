#pragma once

#include "sweep_event.h"
#include <set>

namespace sweep_union
{
	struct SweepEventData;
	typedef sweep_line::SweepEvent<SweepEventData> SweepEvent;
	typedef sweep_line::SweepEventComparer<SweepEventData> SweepEventComparer;
	typedef sweep_line::SegmentComparer<SweepEventData> SegmentComparer;

	/// <summary>
	/// The sweep event data for a fast cascaded union.
	/// </summary>
	struct SweepEventData
	{
		/// <summary>
		/// The number of outside-to-inside edges below this point.
		/// </summary>
		short out_ins_before;

		/// <summary>
		/// The number of inside-to-outside edges that the sweep event's segment represents.
		/// For a normal edge, this is 1 for an inside-to-outside edge, or -1 for an outside-to-inside edge.
		/// </summary>
		short out_ins;

		/// <summary>
		/// If <c>true<c>, the event is already removed from the status line and should not
		/// be processed anymore.
		/// </summary>
		bool overlapped;

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="s_in_outs">The number of inside-to-outside transitions before the edge.</param>
		/// <param name="b_in_out">Whether the edge is an inside-to-outside transition.</param>
		SweepEventData(const short s_out_ins = 0, const short s_out_ins_before = 0)
			: out_ins(s_out_ins), out_ins_before(s_out_ins_before), overlapped(false)
		{
		}
	};
}