#pragma once

#include "gdsii_elements.h"
#include "gdsii_transform.h"

namespace gdsii
{
	/// <summary>
	/// A text element.
	/// </summary>
	struct Text : Element
	{
		/// <summary>
		/// The layer.
		/// </summary>
		short layer;

		/// <summary>
		/// The text type.
		/// </summary>
		short texttype;

		/// <summary>
		/// The presentation.
		/// </summary>
		short presentation;

		/// <summary>
		/// The path type.
		/// </summary>
		short pathtype;

		/// <summary>
		/// The width.
		/// </summary>
		int width;

		/// <summary>
		/// The transform.
		/// </summary>
		Transform strans;

		/// <summary>
		/// The location.
		/// </summary>
		XY coordinate;

		/// <summary>
		/// The string.
		/// </summary>
		string str;

		/// <summary>
		/// Creates a new text.
		/// </summary>
		Text()
			: Element(constants::Text), layer(), texttype(), presentation(), pathtype(), width(), strans(), coordinate(), str()
		{
		}

		void serialize(ostream& stream)
		{
			serialize_header(stream);

			// Layer
			write_short_record(stream, constants::Layer, layer);

			// Text body
			write_short_record(stream, constants::TextType, texttype);

			// Optional presentation
			if (presentation != 0)
				write_short_record(stream, constants::Presentation, presentation);

			// Optional path type
			if (pathtype != 0)
				write_short_record(stream, constants::PathType, pathtype);

			// Optional width
			if (width != 0)
				write_int_record(stream, constants::Width, width);

			// Optional transform
			if (strans.flags != 0 || strans.mag != 0.0 || strans.angle != 0.0)
				strans.serialize(stream);

			// Location
			coordinate.serialize(stream);

			// String
			write_string_record(stream, constants::AsciiString, str);

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

			// Text body
			if (header.type != constants::TextType ||
				!read_short_record(stream, header, texttype))
				return false;

			// Optional presentation
			if (header.type == constants::Presentation &&
				!read_short_record(stream, header, presentation))
				return false;

			// Optional path type
			if (header.type == constants::PathType &&
				!read_short_record(stream, header, pathtype))
				return false;

			// Optional width
			if (header.type == constants::Width &&
				!read_int_record(stream, header, width))
				return false;

			// Optional transform
			if (header.type == constants::Strans &&
				!strans.deserialize(stream, header))
				return false;

			// Location
			if (!coordinate.deserialize(stream, header))
				return false;

			// String
			if (header.type != constants::AsciiString ||
				!read_string_record(stream, header, str))
				return false;

			return deserialize_end(stream, header);
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other text.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Text& other) const {
			return elflags == other.elflags &&
				plex == other.plex &&
				layer == other.layer &&
				texttype == other.texttype &&
				presentation == other.presentation &&
				pathtype == other.pathtype &&
				width == other.width &&
				strans == other.strans &&
				coordinate == other.coordinate &&
				str == other.str;
		}
	};
}