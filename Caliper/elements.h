#pragma once
#include <vector>
#include <map>
using namespace std;

namespace gdsii
{
	namespace constants
	{
		const short Header = 0x0002;
		const short BgnLib = 0x0102;
		const short LibName = 0x0206;
		const short RefLibs = 0x1f06;
		const short Fonts = 0x2006;
		const short AttrTable = 0x2306;
		const short Generations = 0x2202;
		const short Format = 0x3602;
		const short Mask = 0x3706;
		const short EndMask = 0x3800;
		const short Units = 0x0305;
		const short EndLib = 0x0400;
		const short BgnStr = 0x0502;
		const short StrName = 0x0606;
		const short EndStr = 0x0700;
		const short EndEl = 0x1100;
		const short PropAttr = 0x2b02;
		const short PropValue = 0x2c06;

		const short Boundary = 0x0800;
		const short Path = 0x0900;
		const short Sref = 0x0a00;
		const short Aref = 0x0b00;
		const short Text = 0x0c00;
		const short Node = 0x1500;
		const short Box = 0x2d00;
		const short ElFlags = 0x2601;
		const short Plex = 0x2f03;
		const short Layer = 0x0d02;
		const short Datatype = 0x0e02;
		const short XY = 0x1003;
		const short PathType = 0x2102;
		const short Width = 0x0f03;
		const short Sname = 0x1206;
		const short Strans = 0x1a01;
		const short Mag = 0x1b05;
		const short Angle = 0x1c05;
		const short ColRow = 0x1302;
		const short TextType = 0x1602;
		const short Presentation = 0x1701;
		const short AsciiString = 0x1906;
		const short NodeType = 0x2a02;
		const short BoxType = 0x2e02;
	}

	struct XY
	{
		int x;
		int y;

		/// <summary>
		/// Creates a new XY coordinate.
		/// </summary>
		XY()
			: x(), y()
		{
		}
	};

	struct Transform
	{
		short flags;
		double mag;
		double angle;

		/// <summary>
		/// Creates a new transform.
		/// </summary>
		Transform()
			: flags(), mag(), angle()
		{
		}
	};

	struct ColRow
	{
		short cols;
		short rows;

		/// <summary>
		/// Creates a new column-row.
		/// </summary>
		ColRow()
			: cols(), rows()
		{
		}
	};

	struct Element
	{
		short elflags;
		int plex;
		const short type;
		map<short, string> properties;

		/// <summary>
		/// Creates an element.
		/// </summary>
		Element(short s_type)
			: elflags(), plex(), type(s_type), properties()
		{
		}
	};

	struct Boundary : Element
	{
		short layer;
		short datatype;
		vector<XY> points;

		/// <summary>
		/// Creates a new boundary.
		/// </summary>
		Boundary()
			: Element(constants::Boundary), layer(), datatype(), points()
		{
		}
	};

	struct Path : Element
	{
		short layer;
		short datatype;
		short pathtype;
		int width;
		vector<XY> points;

		/// <summary>
		/// Creates a new path.
		/// </summary>
		Path()
			: Element(constants::Path), layer(), datatype(), pathtype(), width(), points()
		{
		}
	};

	struct Sref : Element
	{
		string sname;
		Transform strans;
		XY coordinate;

		/// <summary>
		/// Creates a new structure reference.
		/// </summary>
		Sref()
			: Element(constants::Sref), sname(), strans(), coordinate()
		{
		}
	};

	struct Aref : Element
	{
		string sname;
		Transform strans;
		ColRow colrow;
		XY coordinate;

		/// <summary>
		/// Creates a new array reference.
		/// </summary>
		Aref()
			: Element(constants::Aref), sname(), strans(), colrow(), coordinate()
		{
		}
	};

	struct Text : Element
	{
		short layer;
		short texttype;
		short presentation;
		short pathtype;
		int width;
		Transform strans;
		XY coordinate;
		string str;

		/// <summary>
		/// Creates a new text.
		/// </summary>
		Text()
			: Element(constants::Text), layer(), texttype(), presentation(), pathtype(), width(), strans(), coordinate(), str()
		{
		}
	};

	struct Node : Element
	{
		short layer;
		short nodetype;
		vector<XY> points;

		/// <summary>
		/// Creates a new node.
		/// </summary>
		Node()
			: Element(constants::Node), layer(), nodetype(), points()
		{
		}
	};

	struct Box : Element
	{
		short layer;
		short boxtype;
		XY points[5];

		/// <summary>
		/// Creates a new box.
		/// </summary>
		Box()
			: Element(constants::Box), layer(), boxtype(), points()
		{
		}
	};
}