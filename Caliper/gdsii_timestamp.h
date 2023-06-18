#pragma once

#include "gdsii_io.h"
#include "gdsii_constants.h"

namespace gdsii
{
	/// <summary>
	/// A timestamp.
	/// </summary>
	struct Timestamp
	{
		/// <summary>
		/// The year.
		/// </summary>
		short year;

		/// <summary>
		/// The month.
		/// </summary>
		short month;

		/// <summary>
		/// The day.
		/// </summary>
		short day;

		/// <summary>
		/// The hour.
		/// </summary>
		short hour;

		/// <summary>
		/// The minutes.
		/// </summary>
		short minute;

		/// <summary>
		/// The seconds.
		/// </summary>
		short second;

		/// <summary>
		/// Creates a new timestamp.
		/// </summary>
		Timestamp()
			: year(), month(), day(), hour(), minute(), second()
		{
		}

		/// <summary>
		/// Creates a new timestamp.
		/// </summary>
		/// <param name="s_year">The year.</param>
		/// <param name="s_month">The month.</param>
		/// <param name="s_day">The day.</param>
		/// <param name="s_hour">The hour.</param>
		/// <param name="s_minute">The minute.</param>
		/// <param name="s_second">The second.</param>
		Timestamp(short s_year, short s_month, short s_day, short s_hour, short s_minute, short s_second)
			: year(s_year), month(s_month), day(s_day), hour(s_hour), minute(s_minute), second(s_second)
		{
		}

		/// <summary>
		/// Serializes the timestamp to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			write_short(stream, year);
			write_short(stream, month);
			write_short(stream, day);
			write_short(stream, hour);
			write_short(stream, minute);
			write_short(stream, second);
		}

		/// <summary>
		/// Reads a timestamp from the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="result">The result.</param>
		/// <returns>Returns <c>true</c> if the timestamp could be deserialized; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream)
		{
			if (!read_short(stream, year) ||
				!read_short(stream, month) ||
				!read_short(stream, day) ||
				!read_short(stream, hour) ||
				!read_short(stream, minute) ||
				!read_short(stream, second))
				return false;
			return true;
		}

		/// <summary>
		/// Overload equality.
		/// </summary>
		/// <param name="other">The other timestamp.</param>
		/// <returns>Returns <c>true</c> if the timestamps are equal; otherwise, <c>false</c>.</returns>
		bool operator== (const Timestamp& other) const {
			return (year == other.year) &&
				(month == other.month) &&
				(day == other.day) &&
				(hour == other.hour) &&
				(minute == other.minute) &&
				(second == other.second);
		}
	};

}