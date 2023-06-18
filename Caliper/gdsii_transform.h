#pragma once

#include "gdsii_io.h"
#include "gdsii_constants.h"

namespace gdsii
{
	/// <summary>
	/// A transform.
	/// </summary>
	struct Transform
	{
		/// <summary>
		/// The transform flags.
		/// </summary>
		short flags;

		/// <summary>
		/// The magnification.
		/// </summary>
		double mag;

		/// <summary>
		/// The angle.
		/// </summary>
		double angle;

		/// <summary>
		/// Creates a new transform.
		/// </summary>
		Transform()
			: flags(), mag(), angle()
		{
		}

		/// <summary>
		/// Serializes the transform to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			write_short_record(stream, constants::Strans, flags);
			if (mag != 0.0)
			{
				RecordHeader::serialize(stream, 2 * sizeof(short) + sizeof(double), constants::Mag);
				write_double(stream, mag);
			}
			if (angle != 0.0)
			{
				RecordHeader::serialize(stream, 2 * sizeof(short) + sizeof(double), constants::Angle);
				write_double(stream, angle);
			}
		}

		/// <summary>
		/// Deserializes a transform.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="size">The record size.</param>
		/// <param name="type">The record type.</param>
		/// <param name="result">The result.</param>
		/// <returns>Returns <c>true</c> if the transform was read and the size and type matches; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (header.size != 3 * sizeof(short) || header.type != constants::Strans ||
				!read_short(stream, flags) ||
				!header.deserialize(stream))
				return false;

			// Optional magnitude
			if (header.size == 2 * sizeof(short) + sizeof(double) && header.type == constants::Mag)
			{
				if (!read_double(stream, mag) ||
					!header.deserialize(stream))
					return false;
			}

			// Optional angle
			if (header.size == 2 * sizeof(short) + sizeof(double) && header.type == constants::Angle)
			{
				if (!read_double(stream, angle) ||
					!header.deserialize(stream))
					return false;
			}

			return true;
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other transform.</param>
		/// <returns>Returns <c>true</c> if both transforms are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Transform& other) const {
			return (flags == other.flags) &&
				(mag == other.mag) &&
				(angle == other.angle);
		}
	};
}