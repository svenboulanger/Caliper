#pragma once

#include "gdsii.h"
#include "gdsii_aref.h"
#include "gdsii_elements.h"
#include "gdsii_boundary.h"
#include "gdsii_box.h"
#include "gdsii_node.h"
#include "gdsii_path.h"
#include "gdsii_sref.h"
#include "gdsii_text.h"

namespace gdsii
{
	/// <summary>
	/// A structure.
	/// </summary>
	struct Structure
	{
		/// <summary>
		/// The name of the structure.
		/// </summary>
		string name;

		/// <summary>
		/// The timestamp of last modification.
		/// </summary>
		Timestamp last_modification;

		/// <summary>
		/// The timestamp of last access.
		/// </summary>
		Timestamp last_access;

		/// <summary>
		/// Creates a new structure.
		/// </summary>
		/// <param name="str_name">The name of the structure.</param>
		Structure()
			: name(), elements(), arefs(), boundaries(), boxes(), nodes(), paths(), srefs(), texts()
		{
		}

		/// <summary>
		/// Serializes the structure to the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		void serialize(ostream& stream)
		{
			RecordHeader::serialize(stream, 28, constants::BgnStr);
			last_modification.serialize(stream);
			last_access.serialize(stream);

			// Structure name
			write_string_record(stream, constants::Sname, name);

			// Elements
			for (vector<Element*>::iterator it = elements.begin(); it != elements.end(); it++)
				(*it)->serialize(stream);

			// End str
			RecordHeader::serialize(stream, 2 * sizeof(short), constants::EndStr);
		}

		/// <summary>
		/// Deserializes a structure from the stream.
		/// </summary>
		/// <param name="stream">The stream.</param>
		/// <param name="header">The header.</param>
		/// <returns>Returns <c>true</c> if the structure was read with matching size and type; otherwise, <c>false</c>.</returns>
		bool deserialize(istream& stream, RecordHeader& header)
		{
			if (header.size != 28 || header.type != constants::BgnStr)
				return false;

			// Last modification date
			if (!last_modification.deserialize(stream) ||
				!last_access.deserialize(stream) ||
				!header.deserialize(stream))
				return false;

			// Structure name
			if (header.type != constants::StrName ||
				!read_string_record(stream, header, name))
				return false;

			// Elements
			while (header.type != constants::EndStr)
			{
				switch (header.type)
				{
				case constants::Aref:
					arefs.push_front(Aref());
					if (!arefs.front().deserialize(stream, header))
						return false;
					elements.push_back(&arefs.front());
					break;

				case constants::Boundary:
					boundaries.push_front(Boundary());
					if (!boundaries.front().deserialize(stream, header))
						return false;
					elements.push_back(&boundaries.front());
					break;

				case constants::Box:
					boxes.push_front(Box());
					if (!boxes.front().deserialize(stream, header))
						return false;
					elements.push_back(&boxes.front());
					break;

				case constants::Node:
					nodes.push_front(Node());
					if (!nodes.front().deserialize(stream, header))
						return false;
					elements.push_back(&nodes.front());
					break;

				case constants::Path:
					paths.push_front(Path());
					if (!paths.front().deserialize(stream, header))
						return false;
					elements.push_back(&paths.front());
					break;

				case constants::Sref:
					srefs.push_front(Sref());
					if (!srefs.front().deserialize(stream, header))
						return false;
					elements.push_back(&srefs.front());
					break;

				case constants::Text:
					texts.push_front(Text());
					if (!texts.front().deserialize(stream, header))
						return false;
					elements.push_back(&texts.front());
					break;

				default:
					cout << "Unrecognized type: " << hex << header.type << endl;
					return false;
				}
			}

			if (header.size != 4)
				return false;
			return header.deserialize(stream);
		}

		/// <summary>
		/// Gets an iterator pointing to the start of the elements.
		/// </summary>
		/// <returns>Returns the iterator.</returns>
		vector<Element*>::iterator begin() { return elements.begin(); }

		/// <summary>
		/// Gets an iterator pointing to the end of the elements.
		/// </summary>
		/// <returns>Returns the iterator.</returns>
		vector<Element*>::iterator end() { return elements.end(); }

	private:
		vector<Element*> elements;
		forward_list<Aref> arefs;
		forward_list<Boundary> boundaries;
		forward_list<Box> boxes;
		forward_list<Node> nodes;
		forward_list<Path> paths;
		forward_list<Sref> srefs;
		forward_list<Text> texts;
	};

}