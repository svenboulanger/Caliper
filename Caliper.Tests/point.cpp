#include "pch.h"
#include "CppUnitTest.h"
#include "point_def.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PointTests
{
	TEST_CLASS(Tests)
	{
	public:

		TEST_METHOD(PointConstructor)
		{
			Point a(1, 2);
			Assert::AreEqual(1l, a.x);
			Assert::AreEqual(2l, a.y);
		}
	};
}
