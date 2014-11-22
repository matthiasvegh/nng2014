#ifndef SRC_GROW_HPP
#define SRC_GROW_HPP

#include "Status.hpp"
#include "DumperFunctions.hpp"
#include "PointRange.hpp"
#include <deque>
#include <vector>
#include <algorithm>
#include <iostream>
#include <boost/random/uniform_int_distribution.hpp>
#include <random>

std::size_t getNumberOfPartitions(const Status& status)
{
	int result = 0;
	Array<bool> visited(status.width(), status.height(), false);

	for (Point p: arrayRange(status)) {
		if (!visited[p]) {
			floodFill(status, p, visited);
			++result;
		}
	}
	return result;
}

struct GrowData {
	std::deque<Point> next;
	Point nextSearch;
	bool stuck = false;
	int value;
	std::size_t size = 0;
	std::size_t targetSize = 0;
};

template <typename RandomGenerator>
class Grow {
	Status originalStatus;
	Status& status;
	RandomGenerator& rng;
	Array<bool> visited{status.width(), status.height()};
	std::vector<GrowData> datas;

	void flood(Point p0, GrowData& data, bool updateSize = true)
	{
		std::vector<Point> pointsToVisit{p0};
		pointsToVisit.reserve(status.width()*status.height());

		while (!pointsToVisit.empty()) {
			Point p = pointsToVisit.back();
			pointsToVisit.pop_back();

			if (!arrayAt(visited, p, true)) {

				if (status[p] == data.value) {
					visited[p] = true;
					if (updateSize) {
						++data.size;
					}
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

	void occupy(Point p, GrowData& data)
	{
		status[p] = data.value;
		data.next.push_back(p+Point::p10);
		data.next.push_back(p+Point::pm10);
		data.next.push_back(p+Point::p01);
		data.next.push_back(p+Point::p0m1);
		++data.size;
	}

	void growIteration(GrowData& data)
	{
		Point p = data.next.front();
		data.next.pop_front();

		if (!arrayAt(visited, p, true)) {
			if (status[p] == data.value) {
				flood(p, data);
			} else {
				visited[p] = true;
				occupy(p, data);
			}

		}
	}

	bool checkUnity(Point p, int originalValue)
	{
		Bounds bounds{Point{p.x - 1, p.y - 1}, Point{p.x + 2, p.y + 2}};
		for (Point p: PointRange(bounds.min, bounds.max)) {
			if (arrayAt(status, p, -1) == originalValue) {
				Array<bool> found{status.width(), status.height(), false};
				floodFill(status, p, found, &bounds);

				for (Point p2: PointRange(bounds.min, bounds.max)) {
					if (arrayAt(status, p2, -1) == originalValue && !found[p2]) {
						return false;
					}
				}
				return true;
			}
		}
		return true;
	}

	template <typename Condition>
	bool perturbIteration(GrowData& data, Condition condition)
	{
		visited.fill(false);
		data.next.clear();
		flood(data.nextSearch, data, false);

		std::shuffle(data.next.begin(), data.next.end(), rng);
		for (Point p: data.next) {
			if (!visited[p] && condition(p)) {
				auto originalValue = status[p];
				status[p] = data.value;
				if (checkUnity(p, originalValue)) {
					--datas[originalValue].size;
					++data.size;
					data.nextSearch = p;
					return true;
				} else {
					status[p] = originalValue;
				}
			}
		}

		//std::cerr << "  Stuck.\n";
		return false;
	}

	template <typename Condition, typename Action>
	void iteration(Condition condition, Action action)
	{
		boost::random::uniform_int_distribution<std::size_t> randomId{0, datas.size() - 1};
		while (std::find_if(datas.begin(), datas.end(), condition) != datas.end())
		{
			std::size_t id = randomId(rng);
			auto& data = datas[id];

			if (condition(data)) {
				action(data);
			}
		}
	}

public:
	Grow(Status& status, RandomGenerator& rng):
		originalStatus(status), status(status), rng(rng)
	{

	}

	bool operator()(const std::vector<std::pair<Point, std::size_t>>& startingPoints)
	{
		// Phase 1: initialize
		for (std::size_t i = 0; i < startingPoints.size(); ++i) {
			datas.emplace_back();
			auto& data = datas.back();

			data.value = i;
			data.targetSize = std::min(startingPoints[i].second, status.width()*status.height());
			data.nextSearch = startingPoints[i].first;

			flood(startingPoints[i].first, data);
		}

		// Phase 2: Occupy until desired size is reached
		//iteration([](const GrowData& data)
			//{
				//return !data.next.empty() && data.size < data.targetSize;
			//}, [&](GrowData& data) { growIteration(data); });

		// Phase 3: Occupy all remaining fields
		iteration([](const GrowData& data)
			{
				return !data.next.empty();
			}, [&](GrowData& data) { growIteration(data); });

		// Phase 4: Perturb the edges randomly
		boost::random::uniform_int_distribution<std::size_t> randomId{0, datas.size() - 1};
		std::size_t iterationNumber = status.width() * status.height() / 3;
		for (std::size_t i = 0; i < iterationNumber; ++i) {
			std::size_t id = randomId(rng);

			perturbIteration(datas[id], [&](Point p)
				{
					return originalStatus[p] != status[p];
				});
		}

		// Phase 5: Occupy from each other until equilibrium is reached
		iteration([](const GrowData& data)
			{
				//std::cerr << "--> " << data.value << ": " << data.size <<
						//" -> " << data.targetSize << " " << data.stuck << '\n';
				return !data.stuck && data.size < data.targetSize;
			}, [&](GrowData& data)
			{
				if (!perturbIteration(data, [&](Point p)
					{
						const auto& otherData = datas[status[p]];
						return otherData.size > otherData.targetSize &&
								originalStatus[p] != status[p];
					})) {
					data.stuck = true;
				}
			});

		return std::find_if(datas.begin(), datas.end(), [](const GrowData& data)
			{
				return data.size != data.targetSize;
			}) == datas.end();
	}
};

template <typename RandomGenerator>
bool grow(Status& status, const std::vector<std::pair<Point, std::size_t>>& startingPoints,
		RandomGenerator& rng)
{
	return Grow<RandomGenerator>{status, rng}(startingPoints);
}


#endif /* SRC_GROW_HPP */
