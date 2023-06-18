#pragma once

#include <string>
#include <vector>
#include <forward_list>
#include "gdsii_units.h"
#include "gdsii_timestamp.h"
#include "gdsii_elements.h"
#include "gdsii_io.h"
#include "gdsii_structure.h"
using namespace std;

namespace gdsii
{
	/// <summary>
	/// A library.
	/// </summary>
	struct Library
	{
		short version;
		Timestamp last_modification;
		Timestamp last_access;
		string name;
		Units units;
		vector<Structure> structures;

		/// <summary>
		/// Creates a new library.
		/// </summary>
		Library()
			: version(), last_modification(), last_access(), name(), structures()
		{
		}

		/// <summary>
		/// Serializes the library to a stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			RecordHeader::serialize(stream, 6, constants::Header);
			write_short(stream, version);

			// Expect library
			RecordHeader::serialize(stream, 28, constants::BgnLib);
			last_modification.serialize(stream);
			last_access.serialize(stream);

			// Library name
			write_string_record(stream, constants::LibName, name);

			// Units
			units.serialize(stream);

			// Structures
			for (vector<Structure>::iterator it = structures.begin(); it != structures.end(); it++)
				it->serialize(stream);

			// Write end of library
			RecordHeader::serialize(stream, 4, constants::EndLib);
		}

		/// <summary>
		/// Deserializes the library from a stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		bool deserialize(istream& stream)
		{
			RecordHeader header;
			if (!header.deserialize(stream))
				return false;

			// Version
			if (header.size != 6 || header.type != constants::Header ||
				!read_short(stream, version) ||
				!header.deserialize(stream))
				return false;

			// Begin library
			if (header.size != 28 || header.type != constants::BgnLib ||
				!last_modification.deserialize(stream) ||
				!last_access.deserialize(stream) ||
				!header.deserialize(stream))
				return false;

			// Library name
			if (header.type != constants::LibName ||
				!read_string_record(stream, header, name))
				return false;

			// Units
			if (!units.deserialize(stream, header))
				return false;

			// Read structures
			while (header.type != constants::EndLib)
			{
				structures.push_back(Structure());
				if (!structures.back().deserialize(stream, header))
				{
					cout << "Stuck at " << hex << header.type << endl;
					return false;
				}
			}

			return true;
		}
	};
}