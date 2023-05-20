
#include "connector.h"
#include <algorithm>

void PointChain::init(const Segment& s)
{
	_l.push_back(s.begin());
	_l.push_back(s.end());
}

bool PointChain::link_segment(const Segment& s)
{
	if (s.begin() == _l.front()) {
		if (s.end() == _l.back())
			_closed = true;
		else
			_l.push_front(s.end());
		return true;
	}
	if (s.end() == _l.back()) {
		if (s.begin() == _l.front())
			_closed = true;
		else
			_l.push_back(s.begin());
		return true;
	}
	if (s.end() == _l.front()) {
		if (s.begin() == _l.back())
			_closed = true;
		else
			_l.push_front(s.begin());
		return true;
	}
	if (s.begin() == _l.back()) {
		if (s.end() == _l.front())
			_closed = true;
		else
			_l.push_back(s.end());
		return true;
	}
	return false;
}

bool PointChain::link_pointchain(PointChain& chain)
{
	if (chain._l.front() == _l.back()) {
		chain._l.pop_front();
		_l.splice(_l.end(), chain._l);
		return true;
	}
	if (chain._l.back() == _l.front()) {
		_l.pop_front();
		_l.splice(_l.begin(), chain._l);
		return true;
	}
	if (chain._l.front() == _l.front()) {
		_l.pop_front();
		reverse(chain._l.begin(), chain._l.end());
		_l.splice(_l.begin(), chain._l);
		return true;
	}
	if (chain._l.back() == _l.back()) {
		_l.pop_back();
		reverse(chain._l.begin(), chain._l.end());
		_l.splice(_l.end(), chain._l);
		return true;
	}
	return false;
}

void Connector::add(const Segment& s)
{
	iterator j = _open_polygons.begin();
	while (j != _open_polygons.end())
	{
		if (j->link_segment(s))
		{
			if (j->closed())
				_closed_polygons.splice(_closed_polygons.end(), _open_polygons, j);
			else
			{
				iterator k = j;
				for (++k; k != _open_polygons.end(); k++)
				{
					if (j->link_pointchain(*k))
					{
						_open_polygons.erase(k);
						break;
					}
				}
			}
			return;
		}
		j++;
	}

	_open_polygons.push_back(PointChain());
	_open_polygons.back().init(s);
}

void Connector::to_polygon(Polygon& p)
{
	for (iterator it = begin(); it != end(); it++)
	{
		p.push_back(Contour());
		Contour& contour = p.back();
		for (PointChain::iterator it2 = it->begin(); it2 != it->end(); it2++)
			contour.add(*it2);
	}
}