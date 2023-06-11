#include "pch.h"
#include "polygon_check.h"

void check_martinez_op(Point* p1, int n_p1, Point* p2, int n_p2, BooleanOperation::BooleanOperationType type, Polygon& result)
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
void check_sweep_union(Point* p1, int n_p1, Point* p2, int n_p2, Polygon& result)
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

	sweep_union::PolygonUnion operation = sweep_union::PolygonUnion();
	operation.add(a);
	operation.add(b);
	operation.compute(result);
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