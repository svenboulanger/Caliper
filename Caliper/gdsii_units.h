#pragma once

#include "gdsii_io.h"
#include "gdsii_constants.h"

namespace gdsii
{
	/// <summary>
	/// Units.
	/// </summary>
	struct Units
	{
		/// <summary>
		/// The size of a database unit in user units.
		/// </summary>
		double user_units;

		/// <summary>
		/// The size of a database unit in meters.
		/// </summary>
		double meters;

		/// <summary>
		/// Create units.
		/// </summary>
		Units()
			: user_units(), meters()
		{
		}

		/// <summary>
		/// Create units.
		/// </summary>
		/// <param name="d_db_unit_in_user_units"></param>
		/// <param name="d_db_unit_in_meter"></param>
		Units(double d_user_units, double d_meters)
			: user_units(d_user_units), meters(d_meters)
		{
		}

		/// <summary>
		/// Serializes the units to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			// Write the record
			RecordHeader::serialize(stream, 20, constants::Units);
			write_double(stream, user_units);
			write_double(stream, meters);
		}

		/// <summary>
		/// Reads units from the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="header">The record header.</param>
		/// <returns>Returns <c>true</c> if the units could be read with matching size and type; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (header.size != 20 || header.type != constants::Units ||
				!read_double(stream, user_units) ||
				!read_double(stream, meters))
				return false;
			return header.deserialize(stream);
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other units.</param>
		/// <returns>Returns <c>true</c> if both units are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Units& other) const {
			return (user_units == other.user_units) &&
				(meters == other.meters);
		}
	};
}