#pragma once

#include "gdsii_io.h"

namespace gdsii
{
	namespace constants
	{
		const short Header = 0x0002;
		const short BgnLib = 0x0102;
		const short LibName = 0x0206;
		const short RefLibs = 0x1f06;
		const short Fonts = 0x2006;
		const short AttrTable = 0x2306;
		const short Generations = 0x2202;
		const short Format = 0x3602;
		const short Mask = 0x3706;
		const short EndMask = 0x3800;
		const short Units = 0x0305;
		const short EndLib = 0x0400;
		const short BgnStr = 0x0502;
		const short StrName = 0x0606;
		const short EndStr = 0x0700;
		const short EndEl = 0x1100;
		const short PropAttr = 0x2b02;
		const short PropValue = 0x2c06;

		const short Boundary = 0x0800;
		const short Path = 0x0900;
		const short Sref = 0x0a00;
		const short Aref = 0x0b00;
		const short Text = 0x0c00;
		const short Node = 0x1500;
		const short Box = 0x2d00;
		const short ElFlags = 0x2601;
		const short Plex = 0x2f03;
		const short Layer = 0x0d02;
		const short Datatype = 0x0e02;
		const short XY = 0x1003;
		const short PathType = 0x2102;
		const short Width = 0x0f03;
		const short Sname = 0x1206;
		const short Strans = 0x1a01;
		const short Mag = 0x1b05;
		const short Angle = 0x1c05;
		const short ColRow = 0x1302;
		const short TextType = 0x1602;
		const short Presentation = 0x1701;
		const short AsciiString = 0x1906;
		const short NodeType = 0x2a02;
		const short BoxType = 0x2e02;
	}

	/// <summary>
	/// A record header.
	/// </summary>
	struct RecordHeader
	{
		/// <summary>
		/// Serializes a record to a stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="size">The size.</param>
		/// <param name="type">The type.</param>
		static void serialize(ostream& stream, const short size, const short type)
		{
			write_short(stream, size);
			write_short(stream, type);
		}

		/// <summary>
		/// The record size.
		/// </summary>
		short size;

		/// <summary>
		/// The record id.
		/// </summary>
		short type;

		/// <summary>
		/// Constructor.
		/// </summary>
		RecordHeader()
			: size(), type()
		{
		}

		/// <summary>
		/// Constructor.
		/// </summary>
		/// <param name="s_size">The record size.</param>
		/// <param name="s_id"></param>
		RecordHeader(short s_size, short s_id)
			: size(s_size), type(s_id)
		{
		}

		/// <summary>
		/// Serializes the record header to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			write_short(stream, size);
			write_short(stream, type);
		}

		/// <summary>
		/// Deserializes the record header to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <returns>Returns <c>true</c> if the record header was read; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream)
		{
			if (!read_short(stream, size) ||
				!read_short(stream, type))
				return false;
			return true;
		}

		/// <summary>
		/// Override equality.
		/// </summary>
		/// <param name="other">The other record header.</param>
		/// <returns>Returns <c>true</c> if both are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const RecordHeader& other) const {
			return size == other.size &&
				type == other.type;
		}
	};

	/// <summary>
	/// Writes a 2-byte short for GDSII to the stream with the record attached.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="type">The record type.</param>
	/// <param name="value">The value.</param>
	void write_short_record(ostream& stream, const short type, const short& value);

	/// <summary>
	/// Reads a short and updates the record header from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="header">The record header.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the short could be read with matching size and type; otherwise, <c>false</c>.</returns>
	bool read_short_record(istream& stream, RecordHeader& header, short& result);

	/// <summary>
	/// Writes a 4-byte integer for GDSII to the stream with the record attached.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="type">The record type.</param>
	/// <param name="value">The value.</param>
	void write_int_record(ostream& stream, const short type, const int& value);

	/// <summary>
	/// Reads a 4-byte integer and updates the record header from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="header">The record header.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the integer could be read with matching size and type; otherwise, <c>false</c>.</returns>
	bool read_int_record(istream& stream, RecordHeader& header, int& result);

	/// <summary>
	/// Writes a string for GDSII to the stream with the record attached.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="type">The record type.</param>
	/// <param name="value">The value.</param>
	void write_string_record(ostream& stream, const short type, string& value);

	/// <summary>
	/// Reads a string and updates the record header from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="header">The record header.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the string could be read with matching size and type; otherwise, <c>false</c>.</returns>
	bool read_string_record(istream& stream, RecordHeader& header, string& result);

	/// <summary>
	/// Reads an 8-byte double and updates the record header from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="header">The record header.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the double could be read with matching size and type; otherwise, <c>false</c>.</returns>
	bool read_double_record(istream& stream, RecordHeader& header, double& result);
}