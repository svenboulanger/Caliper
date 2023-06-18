#pragma once

#include "pch.h"
#include "CppUnitTest.h"
#include "point_def.h"
#include "polygon.h"
#include "sweep_martinez.h"
#include "sweep_union.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace martinez;

void check_martinez_op(Point* p1, int n_p1, Point* p2, int n_p2, BooleanOperation::BooleanOperationType type, Polygon& result);
void check_sweep_union(Point* p1, int n_p1, Polygon& result);
void check_sweep_union(Point* p1, int n_p1, Point* p2, int n_p2, Polygon& result);
void check_contour_result(Point* expected, int n_r, Contour& result);