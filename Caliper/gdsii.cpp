#include <iostream>
#include "gdsii.h"
#include "elements.h"

namespace gdsii
{
	bool skip(istream& stream, short size)
	{
		char c;
		for (int i = 0; i < size; i++)
		{
			if (!stream.read(&c, 1))
				return false;
		}
		return true;
	}
	bool read_short(istream& stream, short& result)
	{
		unsigned char buffer[2];
		if (!stream.read((char*)&buffer, 2))
			return false;
		result = (buffer[0] << 8) | buffer[1];
		return true;
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
	bool read_string(istream& stream, int length, string& result)
	{
		length -= 2 * sizeof(short); // Correct for size and header
		char buffer[256];
		if (!stream.read(buffer, length))
			return false;
		result = string(buffer, length);
		return true;
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
	inline bool read_record(istream& stream, short& size, short& type)
	{
		if (!read_short(stream, size))
			return false;
		if (!read_short(stream, type))
			return false;
		return true;
	}

	inline bool read_layer(istream& stream, short& size, short& type, short& layer)
	{
		if (size != 6 || type != constants::Layer)
			return false;
		if (!read_short(stream, layer) ||
			!read_record(stream, size, type))
			return false;
		return true;
	}
	inline bool read_datatype(istream& stream, short& size, short& type, short& datatype)
	{
		if (size != 6 || type != constants::Datatype)
			return false;
		if (!read_short(stream, datatype) ||
			!read_record(stream, size, type))
			return false;
		return true;
	}
	bool read_properties(istream& stream, short& size, short& type, map<short, string>& properties)
	{
		while (size == 6 && type == constants::PropAttr)
		{
			short id;
			string value;

			// Read property attribute
			if (!read_short(stream, id) ||
				!read_record(stream, size, type))
				return false;

			// Read property value (string)
			if (type != constants::PropValue ||
				!read_string(stream, size, value) ||
				!read_record(stream, size, type))
				return false;

			// Update
			properties[id] = value;
		}
	}

	bool read_xy(istream& stream, short& size, short& type, XY& result)
	{
		if (size != 12 || type != constants::XY ||
			!read_int(stream, result.x) ||
			!read_int(stream, result.y) ||
			!read_record(stream, size, type))
			return false;
		return true;
	}
	bool read_xy(istream& stream, short& size, short& type, XY *result)
	{
		if (type != constants::XY)
			return false;
		int count = (size - 2 * sizeof(short)) / sizeof(int) / 2;
		for (int i = 0; i < count; i++)
		{
			if (!read_int(stream, result[i].x) ||
				!read_int(stream, result[i].y))
				return false;
		}
		if (!read_record(stream, size, type))
			return false;
		return true;
	}
	bool read_xy(istream& stream, short& size, short& type, vector<XY> &result)
	{
		if (type != constants::XY)
			return false;
		int count = (size - sizeof(short) * 2) / sizeof(int) / 2;
		for (int i = 0; i < count; i++)
		{
			XY point;
			if (!read_int(stream, point.x) ||
				!read_int(stream, point.y))
				return false;
			result.push_back(point);
		}
		if (!read_record(stream, size, type))
			return false;
		return true;
	}

	bool read_element(istream& stream, short& size, short& type, Element& result)
	{
		if (!read_record(stream, size, type))
			return false;

		// Possible ElFlags
		if (size == 6 && type == constants::ElFlags)
		{
			if (!read_short(stream, result.elflags) ||
				!read_record(stream, size, type))
				return false;
		}

		// Possible Plex
		if (size == 8 && type == constants::Plex)
		{
			if (!read_int(stream, result.plex) ||
				!read_record(stream, size, type))
				return false;
		}
		return true;
	}

	bool read_strans(istream& stream, short& size, short& type, Transform& transform)
	{
		if (size == 6 && type == constants::Strans)
		{
			if (!read_short(stream, transform.flags) ||
				!read_record(stream, size, type))
				return false;

			if (type == constants::Mag)
			{
				if (!read_double(stream, transform.mag))
					return false;
			}
			if (type == constants::Angle)
			{
				if (!read_double(stream, transform.angle))
					return false;
			}
		}
		return true;
	}

	bool read_boundary(istream& stream, Boundary& result)
	{
		short size, type;
		if (!read_element(stream, size, type, result))
			return false;

		// We expect the layer
		if (!read_layer(stream, size, type, result.layer))
			return false;

		// We expect the datatype
		if (!read_datatype(stream, size, type, result.datatype))
			return false;

		// We expect XY coordinates
		if (!read_xy(stream, size, type, result.points))
			return false;

		// Optional properties
		if (!read_properties(stream, size, type, result.properties))
			return false;

		// Expect end of element
		if (size != 4 || type != constants::EndEl)
			return false;
		return true;
	}

	bool read_sref(istream& stream, Sref& result)
	{
		short size, type;
		if (!read_element(stream, size, type, result))
			return false;

		// Read the structure name
		if (type != constants::Sname ||
			!read_string(stream, size, result.sname) ||
			!read_record(stream, size, type))
			return false;

		// Optional strans
		if (!read_strans(stream, size, type, result.strans))
			return false;

		// A single XY is expected
		if (size != 12 || type != constants::XY ||
			!read_int(stream, result.coordinate.x) ||
			!read_int(stream, result.coordinate.y) ||
			!read_record(stream, size, type))
			return false;

		// Optional properties
		if (!read_properties(stream, size, type, result.properties))
			return false;

		// Expect end of element
		if (size != 4 || type != constants::EndEl)
			return false;
		return true;
	}

	bool read_path(istream& stream, Path& result)
	{
		short size, type;
		if (!read_element(stream, size, type, result))
			return false;

		// Expect layer and datatype
		if (!read_layer(stream, size, type, result.layer) ||
			!read_datatype(stream, size, type, result.datatype))
			return false;

		// Optional path type
		if (size == 6 && type == constants::PathType)
		{
			if (!read_short(stream, result.pathtype) ||
				!read_record(stream, size, type))
				return false;
		}

		// Optional width
		if (size == 8 && type == constants::Width)
		{
			if (!read_int(stream, result.width) ||
				!read_record(stream, size, type))
				return false;
		}

		// Expect the coordinates
		if (!read_xy(stream, size, type, result.points))
			return false;

		// Optional properties
		if (!read_properties(stream, size, type, result.properties))
			return false;

		// Expect end of element
		if (size != 4 || type != constants::EndEl)
			return false;
		return true;
	}

	bool read_box(istream& stream, Box& result)
	{
		short size, type;
		if (!read_element(stream, size, type, result))
			return false;

		// Expect a layer
		if (!read_layer(stream, size, type, result.layer))
			return false;

		// Box type
		if (size != 6 && type != constants::BoxType ||
			!read_short(stream, result.boxtype) ||
			!read_record(stream, size, type))
			return false;

		// XY coordinates
		if (size != 44 ||
			!read_xy(stream, size, type, result.points))
			return false;

		// Optional properties
		if (!read_properties(stream, size, type, result.properties))
			return false;

		// Expect end of element
		if (size != 4 || type != constants::EndEl)
			return false;
		return true;
	}

	bool read_text(istream& stream, Text& result)
	{
		short size, type;
		if (!read_element(stream, size, type, result))
			return false;

		// Expect layer
		if (!read_layer(stream, size, type, result.layer))
			return false;

		// Expect text type
		if (size != 6 || type != constants::TextType ||
			!read_short(stream, result.texttype) ||
			!read_record(stream, size, type))
			return false;

		// Optional presentation
		if (size == 6 && type == constants::Presentation)
		{
			if (!read_short(stream, result.presentation) ||
				!read_record(stream, size, type))
				return false;
		}

		// Optional path type
		if (size == 6 && type == constants::PathType)
		{
			if (!read_short(stream, result.pathtype) ||
				!read_record(stream, size, type))
				return false;
		}

		// Optional width
		if (size == 8 && type == constants::Width)
		{
			if (!read_int(stream, result.width) ||
				!read_record(stream, size, type))
				return false;
		}

		// Optional transform
		if (!read_strans(stream, size, type, result.strans))
			return false;

		// Read coordinate
		if (!read_xy(stream, size, type, result.coordinate))
			return false;

		// Read the string
		if (type != constants::AsciiString ||
			!read_string(stream, size, result.str) ||
			!read_record(stream, size, type))
			return false;

		// Read properties
		if (!read_properties(stream, size, type, result.properties))
			return false;

		// Expect end of element
		if (size != 4 || type != constants::EndEl)
			return false;
		return true;
	}

	bool read_structure(istream& stream, Structure& result)
	{
		// Read the name of the structure
		short size, type;
		if (!read_record(stream, size, type))
			return false;

		if (type != constants::StrName ||
			!read_string(stream, size, result.name))
			return false;

		// Read elements
		while (read_record(stream, size, type))
		{
 			switch (type)
			{
			case constants::Boundary:
				if (!read_boundary(stream, result.add_boundary()))
					return false;
				break;

			case constants::Sref:
				if (!read_sref(stream, result.add_sref()))
					return false;
				break;

			case constants::Path:
				if (!read_path(stream, result.add_path()))
					return false;
				break;

			case constants::Box:
				if (!read_box(stream, result.add_box()))
					return false;
				break;

			case constants::Text:
				if (!read_text(stream, result.add_text()))
					return false;
				break;

			case constants::EndStr:
				return size == 4;

			default:
				cout << "Unrecognized type: " << hex << type << endl;
				return false;
			}
		}
		return false;
	}

	bool read_datetime(istream& stream, Timestamp& timestamp)
	{
		if (!read_short(stream, timestamp.year) ||
			!read_short(stream, timestamp.month) ||
			!read_short(stream, timestamp.day) ||
			!read_short(stream, timestamp.hour) ||
			!read_short(stream, timestamp.minute) ||
			!read_short(stream, timestamp.second))
			return false;
		return true;
	}

	bool read_library(istream& stream, Library& result)
	{
		// A library should always begin with a header record
		short size, type;
		if (!read_record(stream, size, type))
			return false;

		// First we expect the header
		if (size != 6 || type != constants::Header || !read_short(stream, result.version))
			return false;
		if (!read_record(stream, size, type))
			return false;

		// Next we expect a library
		if (size != 28 || type != constants::BgnLib ||
			!read_datetime(stream, result.last_modification) ||
			!read_datetime(stream, result.last_access))
			return false;
		if (!read_record(stream, size, type))
			return false;

		// Next we expect a library name
		if (type != constants::LibName)
			return false;
		if (!read_string(stream, size, result.name))
			return false;
		if (!read_record(stream, size, type))
			return false;

		// Optionally: reflibs
		// Optionally: fonts
		// Optionally: attribute table
		// Optionally: generations
		// Optionally: format type

		// Units
		if (size != 20 || type != constants::Units)
			return false;
		if (!read_double(stream, result.units.user_units) ||
			!read_double(stream, result.units.meters))
			return false;

		// Read the structures
		while (read_record(stream, size, type) && size != 0)
		{
			// Detect an endlib record
			if (size == 4 && type == constants::EndLib)
				break;
			else if (size == 28 && type == constants::BgnStr)
			{
				result.structures.push_back(Structure());
				Structure* ptr = &result.structures.back();
				if (!read_datetime(stream, (*ptr).last_modification) ||
					!read_datetime(stream, (*ptr).last_access))
					return false;				
				if (!read_structure(stream, *ptr))
					return false;
			}
			else
				return false;
		}
		return true;
	}
}