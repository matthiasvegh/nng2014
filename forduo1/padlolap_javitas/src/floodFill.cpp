#include "floodFill.hpp"
#include <vector>

void floodFill(const Status& status, Point p0, Array<bool>& result)
{
	std::vector<Point> pointsToVisit;
	pointsToVisit.reserve(status.field.width()*status.field.height());
	pointsToVisit.push_back(p0);
	Value value = status.field[p0];

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (!arrayAt(result, p, true)) {

			if (getValue(status, p) == value) {
				result[p] = true;
				pointsToVisit.push_back(p+Point::p10);
				pointsToVisit.push_back(p+Point::pm10);
				pointsToVisit.push_back(p+Point::p01);
				pointsToVisit.push_back(p+Point::p0m1);
			}
		}
	}
}

