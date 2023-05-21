#include "pch.h"
#include "CppUnitTest.h"
#include "martinez.h"
#include "point_def.h"
#include "polygon_check.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PolygonTests
{
	TEST_CLASS(Tests)
	{
	private:
		void check_intersections(const Point& p1, const Point& p2, const Point& p3, const Point& p4, const int expected_intersections, const Point pi0 = Point(), const Point pi1 = Point())
		{
			Segment a = Segment(p1, p2);
			Segment b = Segment(p3, p4);
			Point po0, po1;
			int points = find_intersection(a, b, po0, po1);

			Assert::AreEqual(expected_intersections, points, L"Number of intersections do not match.");
			if (expected_intersections > 0)
			{
				Assert::AreEqual(pi0.x, po0.x, L"X-coordinate of intersection 1 does not match.");
				Assert::AreEqual(pi0.y, po0.y, L"Y-coordinate of intersection 1 does not match.");
			}
			if (expected_intersections > 1)
			{
				Assert::AreEqual(pi1.x, po1.x, L"X-coordinate of intersection 2 does not match.");
				Assert::AreEqual(pi1.y, po1.y, L"Y-coordinate of intersection 2 does not match.");
			}
		}
		
	public:

		// Trivial tests
		TEST_METHOD(When_Trivial_Expect_Reference01) { check_intersections(Point(), Point(), Point(), Point(), 1, Point()); }
		TEST_METHOD(When_Trivial_Expect_Reference02) { check_intersections(Point(1, 2), Point(1, 2), Point(1, 2), Point(1, 2), 1, Point(1, 2)); }
		TEST_METHOD(When_Trivial_Expect_Reference03) { check_intersections(Point(1, 2), Point(3, 2), Point(1, 2), Point(1, 2), 1, Point(1, 2)); }
		TEST_METHOD(When_Trivial_Expect_Reference04) { check_intersections(Point(1, 2), Point(1, 2), Point(1, 2), Point(3, 2), 1, Point(1, 2)); }

		// Single intersection tests
		TEST_METHOD(When_SingleIntersection_Expect_Reference01) { check_intersections(Point(), Point(2, 0), Point(1, -1), Point(1, 1), 1, Point(1, 0)); }
		TEST_METHOD(When_SingleIntersection_Expect_Reference02) { check_intersections(Point(), Point(2, 0), Point(1, 1), Point(1, -1), 1, Point(1, 0)); }
		TEST_METHOD(When_SingleIntersection_Expect_Reference03) { check_intersections(Point(2, 0), Point(), Point(1, -1), Point(1, 1), 1, Point(1, 0)); }
		TEST_METHOD(When_SingleIntersection_Expect_Reference04) { check_intersections(Point(2, 0), Point(), Point(1, 1), Point(1, -1), 1, Point(1, 0)); }

		// Overlapping tests
		TEST_METHOD(When_Overlap_Expect_Reference01) { check_intersections(Point(), Point(3, 0), Point(1, 0), Point(2, 0), 2, Point(1, 0), Point(2, 0)); }
		TEST_METHOD(When_Overlap_Expect_Reference02) { check_intersections(Point(3, 0), Point(), Point(1, 0), Point(2, 0), 2, Point(1, 0), Point(2, 0)); }
		TEST_METHOD(When_Overlap_Expect_Reference03) { check_intersections(Point(), Point(3, 0), Point(2, 0), Point(1, 0), 2, Point(1, 0), Point(2, 0)); }
		TEST_METHOD(When_Overlap_Expect_Reference04) { check_intersections(Point(), Point(2, 0), Point(1, 0), Point(3, 0), 2, Point(1, 0), Point(2, 0)); }

		TEST_METHOD(When_Overlap_Expect_Reference05) { check_intersections(Point(), Point(0, 3), Point(0, 1), Point(0, 2), 2, Point(0, 1), Point(0, 2)); }
		TEST_METHOD(When_Overlap_Expect_Reference06) { check_intersections(Point(0, 3), Point(), Point(0, 1), Point(0, 2), 2, Point(0, 1), Point(0, 2)); }
		TEST_METHOD(When_Overlap_Expect_Reference07) { check_intersections(Point(), Point(0, 3), Point(0, 2), Point(0, 1), 2, Point(0, 1), Point(0, 2)); }
		TEST_METHOD(When_Overlap_Expect_Reference08) { check_intersections(Point(), Point(0, 2), Point(0, 1), Point(0, 3), 2, Point(0, 1), Point(0, 2)); }

		// Check positive_area conventions
		TEST_METHOD(When_PositiveArea_Expect_Reference01) { Assert::AreEqual(false, positive_area(Point(), Point(1, 0), Point(0, 1))); }
		TEST_METHOD(When_PositiveArea_Expect_Reference02) { Assert::AreEqual(true, positive_area(Point(), Point(1, 0), Point(0, -1))); }

		// Intersections
		TEST_METHOD(When_IntersectionPolygon_Expect_Reference01)
		{
			Point a[4] = { Point(), Point(2, 0), Point(2, 2), Point(0, 2) };
			Point b[4] = { Point(1, 1), Point(3, 1), Point(3, 3), Point(1, 3) };

			// Perform the computation
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Intersection, result);

			// Check the results
			Assert::AreEqual((size_t)1, result.contour_count());
			Point e[] = { Point(2, 2), Point(1, 2), Point(1, 1), Point(2, 1) };
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_IntersectionPolygon_Expect_Reference02)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 1), Point(2, 1), Point(2, 2), Point(1, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Intersection, result);

			// The result is a square with a hole in it
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(b, sizeof(b) / sizeof(Point), result.contour(0));
		}
	};
}
