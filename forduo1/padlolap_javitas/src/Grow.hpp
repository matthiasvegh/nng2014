#ifndef SRC_GROW_HPP
#define SRC_GROW_HPP

#include "Status.hpp"
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>

template <typename T, typename Pred>
void grow(Array<T>& status, Point p0, std::size_t targetSize, Pred isAllowed)
{
	Array<bool> visited{status.width(), status.height()};
	std::vector<Point> pointsToVisit;
	std::deque<Point> next;
	pointsToVisit.reserve(status.width()*status.height());
	pointsToVisit.push_back(p0);
	T value = status[p0];
	std::cerr << "value = " << value << std::endl;
	std::size_t size = 0;
	targetSize = std::min(targetSize, status.width()*status.height());

	while (!pointsToVisit.empty()) {
		Point p = pointsToVisit.back();
		pointsToVisit.pop_back();

		if (!arrayAt(visited, p, true)) {

			if (status[p] == value) {
				visited[p] = true;
				++size;
				pointsToVisit.push_back(p+Point::p10);
				pointsToVisit.push_back(p+Point::pm10);
				pointsToVisit.push_back(p+Point::p01);
				pointsToVisit.push_back(p+Point::p0m1);
			} else {
				next.push_back(p);
			}
		}
	}

	while (!next.empty() && size < targetSize) {
		Point p = next.front();
		next.pop_front();

		if (!arrayAt(visited, p, true)) {
			visited[p] = true;

			if (isAllowed(p)) {
				status[p] = value;
				++size;
				next.push_back(p+Point::p10);
				next.push_back(p+Point::pm10);
				next.push_back(p+Point::p01);
				next.push_back(p+Point::p0m1);
			}
		}
	}
}


#endif /* SRC_GROW_HPP */
