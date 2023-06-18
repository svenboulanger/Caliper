#pragma once

#include <string>
#include <vector>
#include <forward_list>
#include "elements.h"
using namespace std;

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
	};

	/// <summary>
	/// Units.
	/// </summary>
	struct Units
	{
		/// <summary>
		/// The size of a database unit in user units.
		/// </summary>
		double user_units;

		/// <summary>
		/// The size of a database unit in meters.
		/// </summary>
		double meters;

		/// <summary>
		/// Create units.
		/// </summary>
		Units()
			: user_units(), meters()
		{
		}

		/// <summary>
		/// Create units.
		/// </summary>
		/// <param name="d_db_unit_in_user_units"></param>
		/// <param name="d_db_unit_in_meter"></param>
		Units(double d_user_units, double d_meters)
			: user_units(d_user_units), meters(d_meters)
		{
		}
	};

	/// <summary>
	/// A structure.
	/// </summary>
	struct Structure
	{
		string name;
		Timestamp last_modification;
		Timestamp last_access;
		typedef vector<Element*>::iterator iterator;
		
		/// <summary>
		/// Creates a new structure.
		/// </summary>
		/// <param name="str_name">The name of the structure.</param>
		Structure()
			: name(), elements(), boundaries(), srefs(), paths()
		{
		}

		/// <summary>
		/// Adds a boundary to the elements.
		/// </summary>
		/// <returns>Returns the created boundary.</returns>
		Boundary& add_boundary()
		{
			boundaries.push_front(Boundary());
			elements.push_back(&boundaries.front());
			return boundaries.front();
		}

		/// <summary>
		/// Adds a structure reference to the elements.
		/// </summary>
		/// <returns>Returns the created structure reference.</returns>
		Sref& add_sref()
		{
			srefs.push_front(Sref());
			elements.push_back(&srefs.front());
			return srefs.front();
		}

		/// <summary>
		/// Adds a path to the elements.
		/// </summary>
		/// <returns>Returns the created path.</returns>
		Path& add_path()
		{
			paths.push_front(Path());
			elements.push_back(&paths.front());
			return paths.front();
		}

		/// <summary>
		/// Adds a box to the elements.
		/// </summary>
		/// <returns>Returns the created box.</returns>
		Box& add_box()
		{
			boxes.push_front(Box());
			elements.push_back(&boxes.front());
			return boxes.front();
		}

		/// <summary>
		/// Adds a text to the elements.
		/// </summary>
		/// <returns>Returns the created text.</returns>
		Text& add_text()
		{
			texts.push_front(Text());
			elements.push_back(&texts.front());
			return texts.front();
		}

		/// <summary>
		/// Gets an iterator pointing to the start of the elements.
		/// </summary>
		/// <returns>Returns the iterator.</returns>
		iterator begin() { return elements.begin(); }

		/// <summary>
		/// Gets an iterator pointing to the end of the elements.
		/// </summary>
		/// <returns>Returns the iterator.</returns>
		iterator end() { return elements.end(); }

	private:
		vector<Element*> elements;
		forward_list<Boundary> boundaries;
		forward_list<Sref> srefs;
		forward_list<Path> paths;
		forward_list<Box> boxes;
		forward_list<Text> texts;
	};

	/// <summary>
	/// A library.
	/// </summary>
	struct Library
	{
		short version;
		Timestamp last_modification;
		Timestamp last_access;
		string name;
		Units units;
		vector<Structure> structures;

		/// <summary>
		/// Creates a new library.
		/// </summary>
		Library()
			: version(), last_modification(), last_access(), name(), structures()
		{
		}
	};

	/// <summary>
	/// Reads a library from a stream.
	/// </summary>
	/// <param name="stream">The stream.</param>
	bool read_library(istream& stream, Library& result);
}