#pragma once

#include "pch.h"
#include "CppUnitTest.h"
#include "point_def.h"
#include "polygon.h"
#include "martinez.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void check_polygon_op(Point* p1, int n_p1, Point* p2, int n_p2, BooleanOperation::BooleanOperationType type, Polygon& result);

void check_contour_result(Point* expected, int n_r, Contour& result);