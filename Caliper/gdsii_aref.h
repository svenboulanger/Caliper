#pragma once

#include "gdsii_elements.h"
#include "gdsii_colrow.h"

namespace gdsii
{
	/// <summary>
	/// An array structure reference.
	/// </summary>
	struct Aref : Element
	{
		/// <summary>
		/// The structure name.
		/// </summary>
		string sname;

		/// <summary>
		/// The transform.
		/// </summary>
		Transform strans;

		/// <summary>
		/// The number of columns and rows.
		/// </summary>
		ColRow colrow;

		/// <summary>
		/// The coordinate.
		/// </summary>
		XY coordinate;

		/// <summary>
		/// Creates a new array reference.
		/// </summary>
		Aref()
			: Element(constants::Aref), sname(), strans(), colrow(), coordinate()
		{
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			// Structure name
			write_string_record(stream, constants::Sname, sname);

			// Optional transform
			if (strans.flags != 0 || strans.mag != 0.0 || strans.angle != 0.0)
				strans.serialize(stream);

			// Columns and rows
			colrow.serialize(stream);

			// Location
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

			// Columns and rows
			if (!colrow.deserialize(stream, header))
				return false;

			// Location
			if (!coordinate.deserialize(stream, header))
				return false;

			return deserialize_end(stream, header);
		}
	};
}