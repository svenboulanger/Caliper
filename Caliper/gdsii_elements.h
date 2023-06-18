#pragma once
#include <vector>
#include <map>
#include "gdsii_constants.h"
#include "gdsii_io.h"
using namespace std;

namespace gdsii
{
	/// <summary>
	/// An element.
	/// </summary>
	struct Element
	{
		/// <summary>
		/// The ElFlags.
		/// </summary>
		short elflags;

		/// <summary>
		/// The Plex
		/// </summary>
		int plex;

		/// <summary>
		/// The type of element.
		/// </summary>
		const short element_type;

		/// <summary>
		/// The properties.
		/// </summary>
		map<short, string> properties;

		/// <summary>
		/// Creates an element.
		/// </summary>
		Element(short s_type)
			: elflags(), plex(), element_type(s_type), properties()
		{
		}

		/// <summary>
		/// Serializes the elements to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		virtual void serialize(ostream& stream) = 0;

		/// <summary>
		/// Deserializes the element from the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="size">The size.</param>
		/// <param name="type">The type.</param>
		/// <returns>Returns <c>true</c> if the element was read with matching size and type; otherwise, <c>false</c>.</returns>
		virtual bool deserialize(istream& stream, RecordHeader& header) = 0;

	protected:
		bool deserialize_header(istream& stream, RecordHeader& header)
		{
			if (header.size != 2 * sizeof(short) || header.type != element_type ||
				!header.deserialize(stream))
				return false;

			if (header.size == 3 * sizeof(short) && header.type == constants::ElFlags)
			{
				if (!read_short_record(stream, header, elflags))
					return false;
			}
			if (header.size == 2 * sizeof(short) + sizeof(int) && header.type == constants::Plex)
			{
				if (!read_int_record(stream, header, plex))
					return false;
			}

			return true;
		}
		bool deserialize_end(istream& stream, RecordHeader& header)
		{
			while (header.size == 3 * sizeof(short) && header.type == constants::PropAttr)
			{
				// We have a property
				short key;
				if (!read_short_record(stream, header, key))
					return false;

				// Read the value
				string value;
				if (header.type != constants::PropValue ||
					!read_string_record(stream, header, value))
					return false;

				properties[key] = value;
			}

			if (header.size != 2 * sizeof(short) || header.type != constants::EndEl)
				return false;
			return header.deserialize(stream);
		}

		void serialize_header(ostream& stream)
		{
			RecordHeader::serialize(stream, 2 * sizeof(short), element_type);

			// ElFlags
			if (elflags != 0)
			{
				RecordHeader::serialize(stream, 3 * sizeof(short), constants::ElFlags);
				write_short(stream, elflags);
			}

			// Plex
			if (plex != 0)
			{
				RecordHeader::serialize(stream, 2 * sizeof(short) + sizeof(int), constants::Plex);
				write_int(stream, plex);
			}
		}
		void serialize_end(ostream& stream)
		{
			for (map<short, string>::iterator it = properties.begin(); it != properties.end(); it++)
			{
				short key = it->first;
				string value = it->second;
				write_short_record(stream, constants::PropAttr, key);
				write_string_record(stream, constants::PropValue, value);
			}
			RecordHeader::serialize(stream, 2 * sizeof(short), constants::EndEl);
		}
	};
}