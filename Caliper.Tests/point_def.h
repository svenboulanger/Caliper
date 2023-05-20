#pragma once

#include "point.h"
#include "pch.h"
#include "CppUnitTest.h"

namespace Microsoft
{
    namespace VisualStudio
    {
        namespace CppUnitTestFramework
        {
            template<> static std::wstring ToString<Point>(const class Point& p)
            {
                std::wostringstream s;
                s << '(' << p.x << ", " << p.y << ')';
                return s.str();
            }
        }
    }
}