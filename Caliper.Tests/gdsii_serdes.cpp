#include "pch.h"
#include "CppUnitTest.h"
#include "gdsii_io.h"
#include "gdsii.h"
#include "gdsii_def.h"
#include "gdsii_elements.h"
#include "gdsii_colrow.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace gdsii;

namespace GDSIITests
{
	TEST_CLASS(GDSIITests)
	{
	public:

		TEST_METHOD(serdes_short)
		{
			stringstream s;
			short expected = 12345, actual;
			write_short(s, expected);
			read_short(s, actual);
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serded_int)
		{
			stringstream s;
			int expected = 0x12345678, actual;
			write_int(s, expected);
			read_int(s, actual);
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_string)
		{
			stringstream s;
			string expected = string("Hello world"), actual;
			write_string(s, expected);
			read_string(s, 12, actual);
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_double)
		{
			stringstream s;
			double expected = -0.1234, actual;
			write_double(s, expected);
			read_double(s, actual);
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_timestamp)
		{
			stringstream s;
			Timestamp expected = Timestamp(), actual;
			expected.year = 2023;
			expected.month = 6;
			expected.day = 18;
			expected.hour = 14;
			expected.minute = 44;
			expected.second = 23;
			expected.serialize(s);
			Assert::IsTrue(actual.deserialize(s));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_units)
		{
			stringstream s;
			Units expected = Units(), actual;
			expected.user_units = 5e-9;
			expected.meters = 1e-9;
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0); // Expected by deserialization

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_xy)
		{
			stringstream s;
			XY expected = XY(), actual;
			expected.x = 5;
			expected.y = -10;
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0); // Expected by deserialization

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_strans)
		{
			stringstream s;
			Transform expected = Transform(), actual;
			expected.flags = 1;
			expected.mag = 2.0;
			expected.angle = 90.0;
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0); // Expected by deserialization

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_colrow)
		{
			stringstream s;
			ColRow expected = ColRow(), actual;
			expected.cols = 3;
			expected.rows = 4;
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0); // Expected in deserialization

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_aref)
		{
			stringstream s;
			Aref expected = Aref(), actual;
			expected.colrow.cols = 10;
			expected.colrow.rows = 11;
			expected.coordinate.x = 2;
			expected.coordinate.y = 3;
			expected.elflags = 123;
			expected.plex = 5;
			expected.properties[2] = string("hello");
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_boundary)
		{
			stringstream s;
			Boundary expected = Boundary(), actual;
			expected.layer = 5;
			expected.datatype = 6;
			expected.elflags = 13;
			expected.plex = 14;
			expected.points.push_back(XY(2, 3));
			expected.points.push_back(XY(4, 5));
			expected.points.push_back(XY(6, 7));
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_box)
		{
			stringstream s;
			Box expected = Box(), actual;
			expected.layer = 3;
			expected.boxtype = 2;
			expected.elflags = 15;
			expected.points[0] = XY(1, 5);
			expected.points[1] = XY(2, 6);
			expected.points[2] = XY(3, 7);
			expected.points[3] = XY(4, 8);
			expected.points[4] = XY(5, 9);
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_node)
		{
			stringstream s;
			Node expected = Node(), actual;
			expected.layer = 5;
			expected.nodetype = 3;
			expected.points.push_back(XY(1, 2));
			expected.points.push_back(XY(4, 5));
			expected.points.push_back(XY(-4, -5));
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_path)
		{
			stringstream s;
			Path expected = Path(), actual;
			expected.layer = 3;
			expected.datatype = 16;
			expected.elflags = 15;
			expected.pathtype = 2;
			expected.width = 15;
			expected.points.push_back(XY(0, 0));
			expected.points.push_back(XY(5, 5));
			expected.points.push_back(XY(10, 0));
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_sref)
		{
			stringstream s;
			Sref expected = Sref(), actual;
			expected.coordinate = XY(5, 6);
			expected.elflags = 10;
			expected.plex = 11;
			expected.sname = string("structure");
			expected.strans.flags = 5;
			expected.strans.mag = 2;
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}

		TEST_METHOD(serdes_text)
		{
			stringstream s;
			Text expected = Text(), actual;
			expected.coordinate = XY(-5, -6);
			expected.plex = 10;
			expected.layer = 255;
			expected.pathtype = 3;
			expected.presentation = 2;
			expected.properties[155] = "str1";
			expected.str = "label";
			expected.strans.mag = 5;
			expected.width = -2;
			expected.serialize(s);
			RecordHeader::serialize(s, 0, 0);

			RecordHeader header;
			Assert::IsTrue(header.deserialize(s));
			Assert::IsTrue(actual.deserialize(s, header));
			Assert::AreEqual(expected, actual);
		}
	};
}