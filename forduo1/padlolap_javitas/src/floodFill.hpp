#ifndef SRC_STATUS_FLOODFILL_HPP
#define SRC_STATUS_FLOODFILL_HPP

#include "Status.hpp"

struct Bounds {
	Point min;
	Point max;
};

template <typename T>
void floodFill(const Array<T>& status, Point p0, Array<bool>& result, const Bounds* bounds = nullptr)
{
	Bounds realBounds;
	if (bounds) {
		realBounds.min = Point{
			std::max(0, bounds->min.x),
			std::max(0, bounds->min.y)};
		realBounds.max = Point{
			std::min(bounds->max.x, static_cast<int>(status.width())),
			std::max(bounds->max.y, static_cast<int>(status.height()))};
	} else {
		realBounds.min = Point{0, 0};
		realBounds.max = Point{
			static_cast<int>(status.width()),
			static_cast<int>(status.height())};
	}
	std::vector<Point> pointsToVisit;
	pointsToVisit.reserve((realBounds.max.x - realBounds.min.x) * (realBounds.max.y - realBounds.min.y));
	pointsToVisit.push_back(p0);
	Value value = status[p0];

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (p.x >= realBounds.min.x && p.x < realBounds.max.x &&
				p.y >= realBounds.min.y && p.y < realBounds.max.y &&
				!result[p]) {

			if (status[p] == value) {
				result[p] = true;
				pointsToVisit.push_back(p+Point::p10);
				pointsToVisit.push_back(p+Point::pm10);
				pointsToVisit.push_back(p+Point::p01);
				pointsToVisit.push_back(p+Point::p0m1);
			}
		}
	}
}

inline
void floodFill(const Status& status, Point p0, Array<bool>& result)
{
	floodFill(status.field, p0, result);
}


#endif /* SRC_STATUS_FLOODFILL_HPP */
