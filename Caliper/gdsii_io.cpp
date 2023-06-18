#include "gdsii_io.h"

namespace gdsii
{
	void write_short(ostream& stream, const short& value)
	{
		stream << (char)((value >> 8) & 0x0FF);
		stream << (char)(value & 0x0FF);
	}

	bool read_short(istream& stream, short& result)
	{
		unsigned char buffer[2];
		if (!stream.read((char*)&buffer, 2))
			return false;
		result = (buffer[0] << 8) | buffer[1];
		return true;
	}

	void write_int(ostream& stream, const int& value)
	{
		stream << (char)((value >> 24) & 0x0FF);
		stream << (char)((value >> 16) & 0x0FF);
		stream << (char)((value >> 8) & 0x0FF);
		stream << (char)(value & 0x0FF);
	}

	bool read_int(istream& stream, int& result)
	{
		unsigned char buffer[4];
		if (!stream.read((char*)&buffer, 4))
			return false;
		result = (buffer[0] << 24) |
			(buffer[1] << 16) |
			(buffer[2] << 8) |
			buffer[3];
		return true;
	}

	void write_string(ostream& stream, string& value)
	{
		bool is_odd = false;
		for (string::iterator it = value.begin(); it != value.end(); it++)
		{
			stream << *it;
			is_odd = !is_odd;
		}

		// Odd-length strings need to be padded with a 0
		if (is_odd)
			stream << '\0';
	}

	bool read_string(istream& stream, short length, string& result)
	{
		char buffer[256];
		if (!stream.read(buffer, length))
			return false;

		// Strip trailing zero chars
		while (buffer[length - 1] == '\0')
			length--;
		result = string(buffer, length);
		return true;
	}

	void write_double(ostream& stream, const double& value)
	{
		unsigned char buffer[sizeof(double)];
		memcpy(buffer, &value, sizeof(double));
		for (char i = 7; i >= 0; i--)
			stream << buffer[i];
	}

	bool read_double(istream& stream, double& result)
	{
		unsigned char buffer[8];
		if (!stream.read((char*)&buffer, 8))
			return false;
		uint64_t tmp =
			(((uint64_t)buffer[0]) << 56) |
			(((uint64_t)buffer[1]) << 48) |
			(((uint64_t)buffer[2]) << 40) |
			(((uint64_t)buffer[3]) << 32) |
			(((uint64_t)buffer[4]) << 24) |
			(((uint64_t)buffer[5]) << 16) |
			(((uint64_t)buffer[6]) << 8) |
			((uint64_t)buffer[7]);
		memcpy(&result, &tmp, sizeof(double));
		return true;
	}
}