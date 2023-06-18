#pragma once

#include "gdsii_elements.h"
#include "gdsii_xy.h"

namespace gdsii
{
	/// <summary>
	/// A boundary.
	/// </summary>
	struct Boundary : Element
	{
		/// <summary>
		/// The layer.
		/// </summary>
		short layer;

		/// <summary>
		/// The datatype.
		/// </summary>
		short datatype;

		/// <summary>
		/// The points.
		/// </summary>
		vector<XY> points;

		/// <summary>
		/// Creates a new boundary.
		/// </summary>
		Boundary()
			: Element(constants::Boundary), layer(), datatype(), points()
		{
		}

		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (!deserialize_header(stream, header))
				return false;

			// Layer
			if (header.type != constants::Layer ||
				!read_short_record(stream, header, layer))
				return false;

			// Datatype
			if (header.type != constants::Datatype ||
				!read_short_record(stream, header, datatype))
				return false;

			// Read the vector of coordinates
			if (!XY::deserialize(stream, header, points))
				return false;

			return deserialize_end(stream, header);
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			// Layer
			RecordHeader::serialize(stream, 3 * sizeof(short), constants::Layer);
			write_short(stream, layer);

			// Datatype
			RecordHeader::serialize(stream, 3 * sizeof(short), constants::Datatype);
			write_short(stream, datatype);

			// XY coordinates
			RecordHeader::serialize(stream, 2 * sizeof(short) + points.size() * sizeof(int) * 2, constants::XY);
			for (vector<XY>::iterator it = points.begin(); it != points.end(); it++)
				it->serialize_single(stream);

			serialize_end(stream);
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other boundary.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Boundary& other) const {
			return elflags == other.elflags &&
				plex == other.plex &&
				layer == other.layer &&
				datatype == other.datatype &&
				points == other.points &&
				properties == other.properties;
		}
	};
}