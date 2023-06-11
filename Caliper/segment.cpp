#include "segment.h"

inline bool are_collinear(const Point& p0, const Point& p1, const Point& p2)
{
	return (p0.x - p2.x) * (p1.y - p2.y) == (p1.x - p2.x) * (p0.y - p2.y);
}

inline int merge_sort_intersection(const long x0, const long x1, const long x2, const long x3, const Segment& seg0, const Segment& seg1, Point& pi0, Point& pi1)
{
	// This code was generated using a script to avoid mistakes
	if (x0 < x1)
	{
		if (x2 < x3)
		{
			if (x0 < x2)
			{
				if (x1 < x2)
				{
					// [x0, x1, x2, x3]
					if (x1 == x2)
					{
						pi0 = seg0.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x1 < x3)
					{
						// [x0, x2, x1, x3]
						pi0 = seg1.begin();
						if (x2 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
					else
					{
						// [x0, x2, x3, x1]
						pi0 = seg1.begin();
						if (x2 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x3 < x0)
				{
					// [x2, x3, x0, x1]
					if (x3 == x0)
					{
						pi0 = seg1.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x3 < x1)
					{
						// [x2, x0, x3, x1]
						pi0 = seg0.begin();
						if (x0 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
					else
					{
						// [x2, x0, x1, x3]
						pi0 = seg0.begin();
						if (x0 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
				}
			}
		}
		else
		{
			if (x0 < x3)
			{
				if (x1 < x3)
				{
					// [x0, x1, x3, x2]
					if (x1 == x3)
					{
						pi0 = seg0.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x1 < x2)
					{
						// [x0, x3, x1, x2]
						pi0 = seg1.end();
						if (x3 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
					else
					{
						// [x0, x3, x2, x1]
						pi0 = seg1.end();
						if (x3 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x2 < x0)
				{
					// [x3, x2, x0, x1]
					if (x2 == x0)
					{
						pi0 = seg1.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x2 < x1)
					{
						// [x3, x0, x2, x1]
						pi0 = seg0.begin();
						if (x0 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
					else
					{
						// [x3, x0, x1, x2]
						pi0 = seg0.begin();
						if (x0 == x1)
							return 1;
						else
						{
							pi1 = seg0.end();
							return 2;
						}
					}
				}
			}
		}
	}
	else
	{
		if (x2 < x3)
		{
			if (x1 < x2)
			{
				if (x0 < x2)
				{
					// [x1, x0, x2, x3]
					if (x0 == x2)
					{
						pi0 = seg0.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x0 < x3)
					{
						// [x1, x2, x0, x3]
						pi0 = seg1.begin();
						if (x2 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
					else
					{
						// [x1, x2, x3, x0]
						pi0 = seg1.begin();
						if (x2 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x3 < x1)
				{
					// [x2, x3, x1, x0]
					if (x3 == x1)
					{
						pi0 = seg1.end();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x3 < x0)
					{
						// [x2, x1, x3, x0]
						pi0 = seg0.end();
						if (x1 == x3)
							return 1;
						else
						{
							pi1 = seg1.end();
							return 2;
						}
					}
					else
					{
						// [x2, x1, x0, x3]
						pi0 = seg0.end();
						if (x1 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
				}
			}
		}
		else
		{
			if (x1 < x3)
			{
				if (x0 < x3)
				{
					// [x1, x0, x3, x2]
					if (x0 == x3)
					{
						pi0 = seg0.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x0 < x2)
					{
						// [x1, x3, x0, x2]
						pi0 = seg1.end();
						if (x3 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
					else
					{
						// [x1, x3, x2, x0]
						pi0 = seg1.end();
						if (x3 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
				}
			}
			else
			{
				if (x2 < x1)
				{
					// [x3, x2, x1, x0]
					if (x2 == x1)
					{
						pi0 = seg1.begin();
						return 1;
					}
					else
						return 0;
				}
				else
				{
					if (x2 < x0)
					{
						// [x3, x1, x2, x0]
						pi0 = seg0.end();
						if (x1 == x2)
							return 1;
						else
						{
							pi1 = seg1.begin();
							return 2;
						}
					}
					else
					{
						// [x3, x1, x0, x2]
						pi0 = seg0.end();
						if (x1 == x0)
							return 1;
						else
						{
							pi1 = seg0.begin();
							return 2;
						}
					}
				}
			}
		}
	}
}

int find_intersection(const Segment& seg0, const Segment& seg1, Point& pi0, Point& pi1)
{
	const Point& p0 = seg0.begin();
	const Point& p1 = seg1.begin();
	Point d0 = seg0.end() - p0;
	Point d1 = seg1.end() - p1;
	Point e = p1 - p0;

	long m = d0.x * d1.y - d0.y * d1.x;
	if (m == 0)
	{
		// The two lines are parallel
		if (!are_collinear(p0, seg0.end(), p1))
		{
			// The two lines are not collinear
			return 0;
		}

		// The two segments are on one line, go over the different options
		if (d0.x != 0)
		{
			// The segments are not horizontal, so we can simply take a look at the point coordinates
			return merge_sort_intersection(seg0.p1.x, seg0.p2.x, seg1.p1.x, seg1.p2.x, seg0, seg1, pi0, pi1);
		}
		else
		{
			// The segment was horizontal, let's use the X-coordinates instead
			return merge_sort_intersection(seg0.p1.y, seg0.p2.y, seg1.p1.y, seg1.p2.y, seg0, seg1, pi0, pi1);
		}
	}
	else
	{
		Point e = seg1.p1 - seg0.p1;
		long n1 = e.x * d1.y - d1.x * e.y;
		long n2 = e.x * d0.y - d0.x * e.y;
		if (m < 0)
		{
			if (n1 > 0 || n2 > 0 || n1 < m || n2 < m)
				return 0;
			pi0 = Point(seg0.p1.x + (n1 * d0.x) / m, seg0.p1.y + (n1 * d0.y) / m);
		}
		else
		{
			if (n1 < 0 || n2 < 0 || n1 > m || n2 > m)
				return 0;
			pi0 = Point(seg0.p1.x + (n1 * d0.x) / m, seg0.p1.y + (n1 * d0.y) / m);
		}
		return 1;
	}
}
