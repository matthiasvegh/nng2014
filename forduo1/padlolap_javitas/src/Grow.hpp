#ifndef SRC_GROW_HPP
#define SRC_GROW_HPP

#include "Status.hpp"
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/random/uniform_int_distribution.hpp>

template <typename T>
struct GrowData {
	std::deque<Point> next;
	T value;
	std::size_t size = 0;
	std::size_t targetSize = 0;
};

template <typename T, typename Pred, typename RandomGenerator>
bool grow(Array<T>& status, const std::vector<std::pair<Point, std::size_t>>& startingPoints,
		Pred isAllowed, RandomGenerator& rng)
{
	Array<bool> visited{status.width(), status.height()};

	std::vector<GrowData<T>> datas;
	for (const auto& startingPoint: startingPoints) {
		datas.emplace_back();
		auto& data = datas.back();

		std::vector<Point> pointsToVisit;
		pointsToVisit.reserve(status.width()*status.height());
		pointsToVisit.push_back(startingPoint.first);
		data.value = status[startingPoint.first];
		data.targetSize = std::min(startingPoint.second, status.width()*status.height());

		while (!pointsToVisit.empty()) {
			Point p = pointsToVisit.back();
			pointsToVisit.pop_back();

			if (!arrayAt(visited, p, true)) {

				if (status[p] == data.value) {
					visited[p] = true;
					++data.size;
					pointsToVisit.push_back(p+Point::p10);
					pointsToVisit.push_back(p+Point::pm10);
					pointsToVisit.push_back(p+Point::p01);
					pointsToVisit.push_back(p+Point::p0m1);
				} else {
					data.next.push_back(p);
				}
			}
		}
	}

	bool result = true;
	while (!datas.empty()) {
		boost::random::uniform_int_distribution<std::size_t> randomId{0, datas.size() - 1};
		std::size_t id = randomId(rng);
		auto& data = datas[id];

		Point p = data.next.front();
		data.next.pop_front();

		//std::cerr << id << "-" << datas.size() << "(" << data.value << "): " << data.next.size() << " -> ";
		if (!arrayAt(visited, p, true)) {
			visited[p] = true;

			if (isAllowed(p, data.value)) {
				status[p] = data.value;
				data.next.push_back(p+Point::p10);
				data.next.push_back(p+Point::pm10);
				data.next.push_back(p+Point::p01);
				data.next.push_back(p+Point::p0m1);
				++data.size;
			}

		}

		//std::cerr << data.next.size() << std::endl;

		if (data.size == data.targetSize || data.next.empty()) {

			if (data.size != data.targetSize) {
				result = false;
			}

			//std::cerr << id << "(" << data.value << ") finished" << std::endl;
			datas.erase(datas.begin() + id);
		}
	}

	return result;
}


#endif /* SRC_GROW_HPP */
