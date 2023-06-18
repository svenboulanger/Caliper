#pragma once

#include "gdsii_elements.h"

namespace gdsii
{
	/// <summary>
	/// A path.
	/// </summary>
	struct Path : Element
	{
		/// <summary>
		/// The layer.
		/// </summary>
		short layer;

		/// <summary>
		/// The data type.
		/// </summary>
		short datatype;

		/// <summary>
		/// The path type.
		/// </summary>
		short pathtype;

		/// <summary>
		/// The path width.
		/// </summary>
		int width;

		/// <summary>
		/// The path points.
		/// </summary>
		vector<XY> points;

		/// <summary>
		/// Creates a new path.
		/// </summary>
		Path()
			: Element(constants::Path), layer(), datatype(), pathtype(), width(), points()
		{
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			write_short_record(stream, constants::Layer, layer);
			write_short_record(stream, constants::Datatype, layer);

			// Optional path type
			if (pathtype != 0)
				write_short_record(stream, constants::PathType, pathtype);

			// Optional width
			if (width != 0)
				write_int_record(stream, constants::Width, width);

			// Coordinates
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

			// Datatype
			if (header.type != constants::Datatype ||
				!read_short_record(stream, header, datatype))
				return false;

			// Optional path type
			if (header.type == constants::PathType &&
				!read_short_record(stream, header, pathtype))
				return false;

			// Optional width
			if (header.type == constants::Width &&
				!read_int_record(stream, header, width))
				return false;

			// Location
			if (!XY::deserialize(stream, header, points))
				return false;

			return deserialize_end(stream, header);
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other path.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Path& other) const {
			return elflags == other.elflags &&
				plex == other.plex &&
				layer == other.layer &&
				datatype == other.datatype &&
				pathtype == other.pathtype &&
				width == other.width &&
				points == other.points &&
				properties == other.properties;
		}
	};
}