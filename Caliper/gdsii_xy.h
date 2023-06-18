#pragma once

#include <vector>
#include "gdsii_io.h"
#include "gdsii_constants.h"

namespace gdsii
{
	/// <summary>
	/// A structure with an X- and Y-coordinate.
	/// </summary>
	struct XY
	{
		/// <summary>
		/// Reads a fixed number of XY's from the stream. The number of points is equal to
		/// (size - 4) / 8.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="size">The record size.</param>
		/// <param name="type">The record type.</param>
		/// <param name="result">The result.</param>
		/// <returns>Returns <c>true</c> if the XY's could be read and the type matches; otherwise, <c>false</c>.</returns>
		static bool deserialize(istream& stream, RecordHeader& header, XY* result)
		{
			if (header.type != constants::XY)
				return false;
			int count = (header.size - 2 * sizeof(short)) / sizeof(int) / 2;
			for (int i = 0; i < count; i++)
			{
				if (!read_int(stream, result[i].x) ||
					!read_int(stream, result[i].y))
					return false;
			}
			return header.deserialize(stream);
		}

		/// <summary>
		/// Reads a variable number of XY's from the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="header">The record header.</param>
		/// <param name="result">The result.</param>
		/// <returns>Returns <c>true</c> if the XY's could be read and the type matches; otherwise, <c>false</c>.</returns>
		static bool deserialize(istream& stream, RecordHeader& header, vector<XY>& result)
		{
			if (header.type != constants::XY)
				return false;
			int count = (header.size - sizeof(short) * 2) / sizeof(int) / 2;
			for (int i = 0; i < count; i++)
			{
				XY point;
				if (!read_int(stream, point.x) ||
					!read_int(stream, point.y))
					return false;
				result.push_back(point);
			}
			return header.deserialize(stream);
		}

		/// <summary>
		/// Writes a vector of XY's to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="points">The points.</param>
		static void serialize(ostream& stream, vector<XY>& points)
		{
			RecordHeader(2 * sizeof(short) + points.size() * sizeof(int) * 2, constants::XY).serialize(stream);
			for (vector<XY>::iterator it = points.begin(); it != points.end(); it++)
				it->serialize_single(stream);
		}

		/// <summary>
		/// Serializes an array of points to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="size">The number of points.</param>
		/// <param name="points">The points.</param>
		static void serialize(ostream& stream, short size, XY* points)
		{
			RecordHeader(2 * sizeof(short) + size * sizeof(int) * 2, constants::XY).serialize(stream);
			for (short i = 0; i < size; i++)
			{
				points->serialize_single(stream);
				points++;
			}
		}

		/// <summary>
		/// The X-coordinate.
		/// </summary>
		int x;

		/// <summary>
		/// The Y-coordinate.
		/// </summary>
		int y;

		/// <summary>
		/// Creates a new XY coordinate.
		/// </summary>
		XY()
			: x(), y()
		{
		}

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="i_x">The x-coordinate.</param>
		/// <param name="i_y">The y-coordinate.</param>
		XY(int i_x, int i_y)
			: x(i_x), y(i_y)
		{
		}

		/// <summary>
		/// Serializes the XY to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			RecordHeader::serialize(stream, 2 * sizeof(short) + 2 * sizeof(int), constants::XY);
			serialize_single(stream);
		}

		/// <summary>
		/// Serializes the XY to the stream without any header.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize_single(ostream& stream)
		{
			write_int(stream, x);
			write_int(stream, y);
		}

		/// <summary>
		/// Reads a single XY from the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="header">The record header.</param>
		/// <returns>Returns <c>true</c> if the XY could be read and the size/type matches; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (header.size != 12 || header.type != constants::XY ||
				!read_int(stream, x) ||
				!read_int(stream, y))
				return false;
			return header.deserialize(stream);
		}

		/// <summary>
		/// Overrides equality.
		/// </summary>
		/// <param name="other">The other XY.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const XY& other) const {
			return (x == other.x) &&
				(y == other.y);
		}

		/// <summary>
		/// Overrides inequality.
		/// </summary>
		/// <param name="other">The other XY.</param>
		/// <returns>Returns <c>false</c> if both are not equal; otherwise, <c>false</c>.</returns>
		bool operator!= (const XY& other) const {
			return (x != other.x) || (y != other.y);
		}
	};
}