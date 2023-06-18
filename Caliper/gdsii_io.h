#pragma once

#include <string>
#include <iostream>
using namespace std;

namespace gdsii
{
	/// <summary>
	/// Writes a 2-byte short for GDSII to the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="value">The value.</param>
	void write_short(ostream& stream, const short& value);

	/// <summary>
	/// Reads a short from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the short could be read; otherwise, <c>false</c>.</returns>
	bool read_short(istream& stream, short& result);

	/// <summary>
	/// Writes a 4-byte integer for GDSII to the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="value">The value.</param>
	void write_int(ostream& stream, const int& value);

	/// <summary>
	/// Reads a 4-byte integer from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the integer could be read; otherwise, <c>false</c>.</returns>
	bool read_int(istream& stream, int& result);

	/// <summary>
	/// Writes a string for GDSII to the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="value">The value.</param>
	void write_string(ostream& stream, string& value);

	/// <summary>
	/// Reads a string from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="length">The length of the string.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the integer could be read; otherwise, <c>false</c>.</returns>
	bool read_string(istream& stream, short length, string& result);

	/// <summary>
	/// Writes an 8-byte double for GDSII to the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="value">The value.</param>
	void write_double(ostream& stream, const double& value);

	/// <summary>
	/// Reads a double from the stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	/// <param name="result">The result.</param>
	/// <returns>Returns <c>true</c> if the double could be read; otherwise, <c>false</c>.</returns>
	bool read_double(istream& stream, double& result);
}