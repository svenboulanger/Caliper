#include "pch.h"
#include "CppUnitTest.h"
#include "sweep_martinez.h"
#include "point_def.h"
#include "polygon_check.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PolygonTests
{
	TEST_CLASS(MartinezXorTests)
	{
		TEST_METHOD(When_OverlappingBoxes_Expect_Reference01)
		{
			// Boxes with 2 intersections
			/*
			*     +-------+
			*     |       |
			* +---+---+   |
			* |   |   |   |
			* |   +---+---+
			* |       |
			* +-------+
			*/
			Point a[4] = { Point(), Point(2, 0), Point(2, 2), Point(0, 2) };
			Point b[4] = { Point(1, 1), Point(3, 1), Point(3, 3), Point(1, 3) };

			// Perform the computation
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);

			// Check the results
			Assert::AreEqual((size_t)2, result.contour_count());
			Point e[] = { Point(2, 1), Point(1, 1), Point(1, 2), Point(0, 2), Point(0, 0), Point(2, 0) };
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
			Point e2[] = { Point(3, 3), Point(1, 3), Point(1, 2), Point(2, 2), Point(2, 1), Point(3, 1) };
			check_contour_result(e2, sizeof(e2) / sizeof(Point), result.contour(1));
		}
		TEST_METHOD(When_BoxInBox_Expect_Reference02)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 1), Point(2, 1), Point(2, 2), Point(1, 2) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);

			// The result is a square with a hole in it
			Assert::AreEqual((size_t)2, result.contour_count());
			check_contour_result(b, sizeof(b) / sizeof(Point), result.contour(0));
			check_contour_result(a, sizeof(a) / sizeof(Point), result.contour(1));
		}
		TEST_METHOD(When_BoxesOverlapCenterTopOutside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 3), Point(1, 4), Point(2, 4), Point(2, 3) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(2, 3), Point(2, 4), Point(1, 4), Point(1, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapCenterTopInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 2), Point(2, 2), Point(2, 3), Point(1, 3) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(2, 3), Point(2, 2), Point(1, 2), Point(1, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapLeftCenterOutside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(-1, 1), Point(0, 1), Point(0, 2), Point(-1, 2) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3), Point(0, 2), Point(-1, 2), Point(-1, 1), Point(0, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapLeftCenterInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(0, 1), Point(1, 1), Point(1, 2), Point(0, 2) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3), Point(0, 2), Point(1, 2), Point(1, 1), Point(0, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapCenterBottomOutside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 0), Point(1, -1), Point(2, -1), Point(2, 0) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(1, 0), Point(1, -1), Point(2, -1), Point(2, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapCenterBottomInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(1, 0), Point(2, 0), Point(2, 1), Point(1, 1) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(1, 0), Point(1, 1), Point(2, 1), Point(2, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapRightCenterOutside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(3, 1), Point(4, 1), Point(4, 2), Point(3, 2) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 1), Point(4, 1), Point(4, 2), Point(3, 2), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapRightCenterInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(2, 1), Point(3, 1), Point(3, 2), Point(2, 2) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 1), Point(2, 1), Point(2, 2), Point(3, 2), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapLeftTopInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(0, 2), Point(1, 2), Point(1, 3), Point(0, 3) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(1, 3), Point(1, 2), Point(0, 2) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapLeftBottomInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(1, 0), Point(3, 0), Point(3, 3), Point(0, 3), Point(0, 1), Point(1, 1) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapRightBottomInside_Expect_Reference)
		{
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(2, 0), Point(3, 0), Point(3, 1), Point(2, 1) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(2, 0), Point(2, 1), Point(3, 1), Point(3, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
		TEST_METHOD(When_BoxesOverlapRightTopInside_Expect_Reference)
		{
			// Box in a box (testing overlapping segments)
			Point a[] = { Point(0, 0), Point(3, 0), Point(3, 3), Point(0, 3) };
			Point b[] = { Point(2, 2), Point(3, 2), Point(3, 3), Point(2, 3) };
			Polygon result = Polygon();
			check_martinez_op(a, sizeof(a) / sizeof(Point), b, sizeof(b) / sizeof(Point), BooleanOperation::Xor, result);
			Point e[] = { Point(0, 0), Point(3, 0), Point(3, 2), Point(2, 2), Point(2, 3), Point(0, 3) };
			Assert::AreEqual((size_t)1, result.contour_count());
			check_contour_result(e, sizeof(e) / sizeof(Point), result.contour(0));
		}
	};
}