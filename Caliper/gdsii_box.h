#pragma once

#include "gdsii_elements.h"

namespace gdsii
{
	/// <summary>
	/// A box.
	/// </summary>
	struct Box : Element
	{
		short layer;
		short boxtype;
		XY points[5];

		/// <summary>
		/// Creates a new box.
		/// </summary>
		Box()
			: Element(constants::Box), layer(), boxtype(), points()
		{
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			// Layer
			write_short_record(stream, constants::Layer, layer);

			// Box type
			write_short_record(stream, constants::BoxType, boxtype);

			// XY
			XY::serialize(stream, 5, points);

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

			// Box type
			if (header.type != constants::BoxType ||
				!read_short_record(stream, header, boxtype))
				return false;

			// XY
			if (header.size != 2 * sizeof(short) + 5 * sizeof(int) * 2 ||
				!XY::deserialize(stream, header, points))
				return false;

			return deserialize_end(stream, header);
		}

		/// <summary>
		/// Override equality.
		/// </summary>
		/// <param name="other">The other box.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Box& other) const {
			if (elflags != other.elflags ||
				plex != other.plex ||
				layer != other.layer ||
				boxtype != other.boxtype ||
				properties != other.properties)
				return false;

			for (char i = 0; i < 5; i++)
			{
				if (points[i] != other.points[i])
					return false;
			}
			return true;
		}
	};
}