#pragma once

#include "gdsii.h"
#include "gdsii_colrow.h"
#include "pch.h"
#include "CppUnitTest.h"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static std::wstring ToString<gdsii::Timestamp>(const struct gdsii::Timestamp& ts)
			{
				std::wostringstream s;
				s << "ts(" << ts.year << ' / ' << ts.month << ' / ' << ts.day << ' ';
				s << ts.hour << ':' << ts.minute << ':' << ts.second << ')';
				return s.str();
			};
			template<> static std::wstring ToString<gdsii::Units>(const struct gdsii::Units& u)
			{
				std::wostringstream s;
				s << "units(" << u.user_units << ", " << u.meters << ')';
				return s.str();
			}
			template<> static std::wstring ToString<gdsii::XY>(const struct gdsii::XY& xy)
			{
				std::wostringstream s;
				s << "xy(" << xy.x << ", " << xy.y << ')';
				return s.str();
			}
			template<> static std::wstring ToString<gdsii::Transform>(const struct gdsii::Transform& tf)
			{
				std::wostringstream s;
				s << "tf(" << tf.flags << ", " << tf.mag << ", " << tf.angle << ')';
				return s.str();
			}
			template<> static std::wstring ToString<gdsii::ColRow>(const struct gdsii::ColRow& cr)
			{
				std::wostringstream s;
				s << "colrow(" << cr.cols << ", " << cr.rows << ')';
				return s.str();
			}
		}
	}
}