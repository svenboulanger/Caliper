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
	};
}