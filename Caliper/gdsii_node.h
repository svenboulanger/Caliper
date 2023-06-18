#pragma once

#include "gdsii_elements.h"

namespace gdsii
{
	/// <summary>
	/// A node.
	/// </summary>
	struct Node : Element
	{
		short layer;
		short nodetype;
		vector<XY> points;

		/// <summary>
		/// Creates a new node.
		/// </summary>
		Node()
			: Element(constants::Node), layer(), nodetype(), points()
		{
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			// Layer
			write_short_record(stream, constants::Layer, layer);

			// Node type
			write_short_record(stream, constants::NodeType, nodetype);

			// points
			XY::serialize(stream, points);

			serialize_end(stream);
		}

		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (!deserialize_header(stream, header))
				return false;

			// Layer
			if (header.type != constants::Layer ||
				!read_short_record(stream, header, layer))
				return false;

			// Node type
			if (header.type != constants::NodeType ||
				!read_short_record(stream, header, nodetype))
				return false;

			// Points
			if (!XY::deserialize(stream, header, points))
				return false;

			return deserialize_end(stream, header);
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other node.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Node& other) const {
			return elflags == other.elflags &&
				plex == other.plex &&
				layer == other.layer &&
				nodetype == other.nodetype &&
				points == other.points &&
				properties == other.properties;
		}
	};
}