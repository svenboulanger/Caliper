#pragma once

#include "gdsii_elements.h"
#include "gdsii_transform.h"

namespace gdsii
{
	/// <summary>
	/// A structure reference.
	/// </summary>
	struct Sref : Element
	{
		/// <summary>
		/// The name of the structure being referenced.
		/// </summary>
		string sname;

		/// <summary>
		/// The transform.
		/// </summary>
		Transform strans;

		/// <summary>
		/// The location.
		/// </summary>
		XY coordinate;

		/// <summary>
		/// Creates a new structure reference.
		/// </summary>
		Sref()
			: Element(constants::Sref), sname(), strans(), coordinate()
		{
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			// Structure name
			write_string_record(stream, constants::Sname, sname);

			// Optional s-trans
			if (strans.flags != 0 || strans.mag != 0.0 || strans.angle != 0.0)
				strans.serialize(stream);

			// Single XY
			coordinate.serialize(stream);

			serialize_end(stream);
		}

		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (!deserialize_header(stream, header))
				return false;

			// Structure name
			if (header.type != constants::Sname ||
				!read_string_record(stream, header, sname))
				return false;

			// Optional transform
			if (header.type == constants::Strans &&
				!strans.deserialize(stream, header))
				return false;

			// Location
			if (!coordinate.deserialize(stream, header))
				return false;

			return deserialize_end(stream, header);
		}

		/// <summary>
		/// Overloads equality.
		/// </summary>
		/// <param name="other">The other structure reference.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Sref& other) const {
			return elflags == other.elflags &&
				plex == other.plex &&
				strans == other.strans &&
				coordinate == other.coordinate &&
				sname == other.sname &&
				properties == other.properties;
		}
	};
}