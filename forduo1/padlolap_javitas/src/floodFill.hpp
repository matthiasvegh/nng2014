#ifndef SRC_STATUS_FLOODFILL_HPP
#define SRC_STATUS_FLOODFILL_HPP

#include "Status.hpp"

template <typename T>
void floodFill(const Array<T>& status, Point p0, Array<bool>& result, int* size = nullptr)
{
	std::vector<Point> pointsToVisit;
	pointsToVisit.reserve(status.width()*status.height());
	pointsToVisit.push_back(p0);
	Value value = status[p0];

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (!arrayAt(result, p, true)) {

			if (arrayAt(status, p, -1) == value) {
				if (size) {
					++*size;
				}
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
void floodFill(const Status& status, Point p0, Array<bool>& result, int* size = nullptr)
{
	floodFill(status.field, p0, result, size);
}


#endif /* SRC_STATUS_FLOODFILL_HPP */
