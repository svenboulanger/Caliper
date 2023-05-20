#include "pch.h"
#include "CppUnitTest.h"
#include "martinez.h"
#include "point_def.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntersectionTests
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
		void check_polygon_op(Point* p1, int n_p1, Point* p2, int n_p2, BooleanOperation::BooleanOperationType type, Polygon& result)
		{
			// Make the first polygon
			Contour c1 = Contour();
			for (int i = 0; i < n_p1; i++)
			{
				c1.add(*p1);
				p1++;
			}
			Polygon a = Polygon();
			a.push_back(c1);

			// Make the second polygon
			Contour c2 = Contour();
			for (int i = 0; i < n_p2; i++)
			{
				c2.add(*p2);
				p2++;
			}
			Polygon b = Polygon();
			b.push_back(c2);

			BooleanOperation operation = BooleanOperation(a, b);
			operation.compute(type, result);
		}
		void check_contour_result(Point* expected, int n_r, Contour& result)
		{
			Assert::AreEqual((size_t)n_r, result.vertex_count(), L"Wrong number of vertices");

			// We will allow an offset in indices
			int offset;
			for (offset = 0; offset < result.vertex_count(); offset++)
			{
				if (*expected == result.vertex(offset))
					break;
			}
			// Note: a vertex should never appear twice in the same polygon, the result
			// should then be a different contour!
			Assert::AreNotEqual(result.vertex_count(), (size_t)offset, L"No matching vertex");
			for (int i = 0; i < n_r; i++)
			{
				Assert::AreEqual(*expected, result.vertex((offset + i) % n_r), L"Wrong vertex");
				expected++;
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

		// Polygon boolean operations
		TEST_METHOD(When_UnionPolygon_Expect_Reference01)
		{
			Point a[4] = { Point(), Point(2, 0), Point(2, 2), Point(0, 2) };
			Point b[4] = { Point(1, 1), Point(3, 1), Point(3, 3), Point(1, 3) };

			// Perform the computation
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);

			// Check the results
			Point e[] = { Point(3, 3), Point(1, 3), Point(1, 2), Point(0, 2), Point(0, 0), Point(2, 0), Point(2, 1), Point(3, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionPolygon_Expect_Reference02)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 1), Point(2, 1), Point(2, 2), Point(1, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);

			// We should just get a back
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(a, sizeof(a) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapCenterTopOutside_Expect_Reference)
		{
			// Box touching box outside center top
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 3), Point(1, 4), Point(2, 4), Point(2, 3) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(2, 3), Point(2, 4), Point(1, 4), Point(1, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapCenterTopInside_Expect_Reference)
		{
			// Box touching box inside center top
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 1), Point(2, 1), Point(2, 3), Point(1, 3) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(2, 3), Point(1, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapLeftCenterOutside_Expect_Reference)
		{
			// Box touching box outside center left
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(-1, 1), Point(0, 1), Point(0, 2), Point(-1, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3), Point(0, 2), Point(-1, 2), Point(-1, 1), Point(0, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapLeftCenterInside_Expect_Reference)
		{
			// Box touching box inside center left
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(0, 1), Point(1, 1), Point(1, 2), Point(0, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3), Point(0, 2), Point(0, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapCenterBottomOutside_Expect_Reference)
		{
			// Box touching box outside center bottom
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 0), Point(1, -1), Point(2, -1), Point(2, 0) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(1, 0), Point(1, -1), Point(2, -1), Point(2, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapCenterBottomInside_Expect_Reference)
		{
			// Box touching box inside center bottom
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 0), Point(2, 0), Point(2, 1), Point(1, 1) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(1, 0), Point(2, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapRightCenterOutside_Expect_Reference)
		{
			// Box touching box outside center right
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(3, 1), Point(4, 1), Point(4, 2), Point(3, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 1), Point(4, 1), Point(4, 2), Point(3, 2), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapRightCenterInside_Expect_Reference)
		{
			// Box touching box inside center left
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(2, 1), Point(3, 1), Point(3, 2), Point(2, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 1), Point(3, 2), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapCenterCenter_Expect_Reference)
		{
			// Box in a box (testing overlapping segments)
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 2), Point(2, 2), Point(2, 3), Point(1, 3) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(2, 3), Point(1, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapTopLeftInside_Expect_Reference)
		{
			// Box in a box (testing overlapping segments)
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(0, 2), Point(1, 2), Point(1, 3), Point(0, 3) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(1, 3), Point(0, 3), Point(0, 2) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapBottomLeftInside_Expect_Reference)
		{
			// Box in a box (testing overlapping segments)
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(1, 0), Point(3, 0), Point(3, 3), Point(0, 3), Point(0, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapBottomRightInside_Expect_Reference)
		{
			// Box in a box (testing overlapping segments)
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(2, 0), Point(3, 0), Point(3, 1), Point(2, 1) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(2, 0), Point(3, 0), Point(3, 1), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionBoxesOverlapTopRightInside_Expect_Reference)
		{
			// Box in a box (testing overlapping segments)
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(2, 2), Point(3, 2), Point(3, 3), Point(2, 3) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 2), Point(3, 3), Point(2, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_UnionPolygonsGithub01_Expect_Reference)
		{
			// https://github.com/w8r/martinez/issues/110
			Point a[] = { Point(115,96), Point(140,206), Point(120,210), Point(125,250), Point(80,300) };
			Point b[] = { Point(111,228), Point(129,192), Point(309,282) };

			// Perform the computation
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Union, result);
			Point r[] = { Point(137, 196), Point(115, 96), Point(80, 300), Point(125, 250), Point(122, 231), Point(309, 282) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(r, sizeof(r) / sizeof(Point), result.contour(0));
		}

		// Xor operation
		TEST_METHOD(When_XorPolygon_Expect_Reference01)
		{
			Point a[4] = { Point(), Point(2, 0), Point(2, 2), Point(0, 2) };
			Point b[4] = { Point(1, 1), Point(3, 1), Point(3, 3), Point(1, 3) };

			// Perform the computation
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);

			// Check the results
			Assert::AreEqual((size_t)2, result.contour_count());
			Point e[] = { Point(2, 1), Point(1, 1), Point(1, 2), Point(0, 2), Point(0, 0), Point(2, 0) };
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
			Point e2[] = { Point(3, 3), Point(1, 3), Point(1, 2), Point(2, 2), Point(2, 1), Point(3, 1) };
			check_contour_result(e2, sizeof(e2) / sizeof(Point), result.contour(1));
		}
		TEST_METHOD(When_XorPolygon_Expect_Reference02)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 1), Point(2, 1), Point(2, 2), Point(1, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);

			// The result is a square with a hole in it
			Assert::AreEqual((size_t)2, result.contour_count());
			check_contour_result(b, sizeof(b) / sizeof(Point), result.contour(0));
			check_contour_result(a, sizeof(a) / sizeof(Point), result.contour(1));
		}

		// Difference operations
		TEST_METHOD(When_DifferencePolygon_Expect_Reference01)
		{
			Point a[4] = { Point(), Point(2, 0), Point(2, 2), Point(0, 2) };
			Point b[4] = { Point(1, 1), Point(3, 1), Point(3, 3), Point(1, 3) };

			// Perform the computation
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Difference, result);

			// Check the results
			Assert::AreEqual((size_t)1, result.contour_count());
			Point e[] = { Point(2, 1), Point(1, 1), Point(1, 2), Point(0, 2), Point(0, 0), Point(2, 0) };
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_DifferencePolygon_Expect_Reference02)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 1), Point(2, 1), Point(2, 2), Point(1, 2) };
			Polygon result = Polygon();
			check_polygon_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Difference, result);

			// The result is a square with a hole in it
			Assert::AreEqual((size_t)2, result.contour_count());
			check_contour_result(b, sizeof(b) / sizeof(Point), result.contour(0));
			check_contour_result(a, sizeof(a) / sizeof(Point), result.contour(1));
		}

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
