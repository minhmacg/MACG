#ifndef M_GEO_H
#define M_GEO_H

#include <tuple>
#include <vector>
#include <algorithm>

struct point2d
{
	float x,y;

	float distance(const point2d&)
	{
	};

	point2d operator + (const point2d& p) 
	{
		return point2d{.x = x + p.x, .y = y + p.y};
	}
	point2d operator - (const point2d& p) 
	{
		return point2d{.x = x - p.x, .y = y - p.y};
	}
	point2d operator * (const point2d& p) 
	{
		return point2d{.x = x * p.x, .y = y * p.y};
	}
	friend std::ostream& operator << (std::ostream& os, point2d& p)
	{
		return os << p.x << ", " << p.y;
	};
};

struct rectangle
{
	//pos start at upleft
	float x,y,w,h;

	rectangle operator * (const float& scale)
	{
		return {x*2, y*2, w*2, h*2};
	};	
	friend std::ostream& operator << (std::ostream& os, rectangle& r)
	{
		return os << r.x << ", " << r.y << ", " << r.w << ", " << r.h;
	};
};
struct circle
{
	float x,y;
	float radius;
};


#endif
