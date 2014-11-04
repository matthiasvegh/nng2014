#include "floodFill.hpp"
#include <vector>

void floodFill(const Status& status, Point p0, Array<bool>& result)
{
	result.fill(false);

	std::vector<Point> pointsToVisit;
	pointsToVisit.reserve(status.width()*status.height());
	pointsToVisit.push_back(p0);
	Value value = status[p0];

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (!arrayAt(result, p, true)) {

			if (value(status, p) == value) {
				result[p] = true;
				pointsToVisit.push_back(p+Point::p10);
				pointsToVisit.push_back(p+Point::pm10);
				pointsToVisit.push_back(p+Point::p01);
				pointsToVisit.push_back(p+Point::p0m1);
			}
		}
	}
}

