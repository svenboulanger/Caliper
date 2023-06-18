#pragma once

#include "gdsii.h"
#include "gdsii_colrow.h"
#include "pch.h"
#include "CppUnitTest.h"
using namespace std;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static wstring ToString<gdsii::Timestamp>(const struct gdsii::Timestamp& ts)
			{
				wostringstream s;
				s << "ts(" << ts.year << ' / ' << ts.month << ' / ' << ts.day << ' ';
				s << ts.hour << ':' << ts.minute << ':' << ts.second << ')';
				return s.str();
			};
			template<> static wstring ToString<gdsii::Units>(const struct gdsii::Units& u)
			{
				wostringstream s;
				s << "units(" << u.user_units << ", " << u.meters << ')';
				return s.str();
			}
			template<> static wstring ToString<gdsii::XY>(const struct gdsii::XY& xy)
			{
				wostringstream s;
				s << "xy(" << xy.x << ", " << xy.y << ')';
				return s.str();
			}
			template<> static wstring ToString<gdsii::Transform>(const struct gdsii::Transform& tf)
			{
				wostringstream s;
				s << "tf(" << tf.flags << ", " << tf.mag << ", " << tf.angle << ')';
				return s.str();
			}
			template<> static wstring ToString<gdsii::ColRow>(const struct gdsii::ColRow& cr)
			{
				wostringstream s;
				s << "colrow(" << cr.cols << ", " << cr.rows << ')';
				return s.str();
			}
			template<> static wstring ToString<gdsii::Aref>(const struct gdsii::Aref& aref)
			{
				wostringstream s;
				s << "aref";
				return s.str();
			}
			template<> static wstring ToString<gdsii::Boundary>(const struct gdsii::Boundary& boundary)
			{
				wostringstream s;
				s << "boundary";
				return s.str();
			}
			template<> static wstring ToString<gdsii::Box>(const struct gdsii::Box& box)
			{
				wostringstream s;
				s << "box";
				return s.str();
			}
			template<> static wstring ToString<gdsii::Node>(const struct gdsii::Node& node)
			{
				wostringstream s;
				s << "node";
				return s.str();
			}
			template<> static wstring ToString<gdsii::Path>(const struct gdsii::Path& path)
			{
				wostringstream s;
				s << "path";
				return s.str();
			}
			template<> static wstring ToString<gdsii::Sref>(const struct gdsii::Sref& path)
			{
				wostringstream s;
				s << "sref";
				return s.str();
			}
			template<> static wstring ToString<gdsii::Text>(const struct gdsii::Text& path)
			{
				wostringstream s;
				s << "text";
				return s.str();
			}
		}
	}
}