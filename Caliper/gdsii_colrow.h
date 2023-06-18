#pragma once

#include "gdsii.h"
#include "gdsii_constants.h"

namespace gdsii
{
	/// <summary>
	/// Number of columns and rows.
	/// </summary>
	struct ColRow
	{
		/// <summary>
		/// The number of columns.
		/// </summary>
		short cols;

		/// <summary>
		/// The number of rows.
		/// </summary>
		short rows;

		/// <summary>
		/// Creates a new column-row.
		/// </summary>
		ColRow()
			: cols(), rows()
		{
		}

		/// <summary>
		/// Serializes the columns and rows to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			RecordHeader::serialize(stream, 4 * sizeof(short), constants::ColRow);
			write_short(stream, cols);
			write_short(stream, rows);
		}

		/// <summary>
		/// Deserializes columns and rows from a streamm.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="header">The record header.</param>
		/// <returns>Returns <c>true</c> if the columns and rows were read and the size and type matches; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (header.size != 4 * sizeof(short) || header.type != constants::ColRow ||
				!read_short(stream, cols) ||
				!read_short(stream, rows) ||
				!header.deserialize(stream))
				return false;
			return true;
		}

		/// <summary>
		/// Overloads equality.
		/// </summary>
		/// <param name="other">The other columns and rows.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const ColRow& other) const {
			return (cols == other.cols) && (rows == other.rows);
		}
	};
}