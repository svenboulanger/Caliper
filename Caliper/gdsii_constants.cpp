#include "gdsii_constants.h"

namespace gdsii
{
	void write_short_record(ostream& stream, const short type, const short& value)
	{
		RecordHeader(3 * sizeof(short), type).serialize(stream);
		write_short(stream, value);
	}

	bool read_short_record(istream& stream, RecordHeader& header, short& result)
	{
		if (header.size != 3 * sizeof(short) ||
			!read_short(stream, result) ||
			!header.deserialize(stream))
			return false;
		return true;
	}


	void write_int_record(ostream& stream, const short type, const int& value)
	{
		RecordHeader(2 * sizeof(short) + sizeof(int), type).serialize(stream);
		write_int(stream, value);
	}

	bool read_int_record(istream& stream, RecordHeader& header, int& result)
	{
		if (header.size != 2 * sizeof(short) + sizeof(int) ||
			!read_int(stream, result) ||
			!header.deserialize(stream))
			return false;
		return true;
	}

	void write_string_record(ostream& stream, const short type, string& value)
	{
		short size = 2 * sizeof(short) + value.length();
		if (size % 2 == 1)
			size++;
		RecordHeader(size, type).serialize(stream);
		write_string(stream, value);
	}

	bool read_string_record(istream& stream, RecordHeader& header, string& result)
	{
		if (!read_string(stream, header.size - 2 * sizeof(short), result) ||
			!header.deserialize(stream))
			return false;
		return true;
	}

	bool read_double_record(istream& stream, RecordHeader& header, double& result)
	{
		if (header.size != 2 * sizeof(short) + sizeof(double) ||
			!read_double(stream, result) ||
			!header.deserialize(stream))
			return false;
		return true;
	}
}