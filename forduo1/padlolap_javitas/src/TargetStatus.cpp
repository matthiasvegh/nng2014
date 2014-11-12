#include "TargetStatus.hpp"
#include "floodFill.hpp"
#include "Grow.hpp"
#include "DumperFunctions.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <stdexcept>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

namespace {

struct ColorData {
	std::size_t numberOfTiles = 0;
	Point referencePoint;
};

}

Status findTargetStatus(const Status& status, unsigned seed)
{
	ColorData data[3];
	for (Point p: arrayRange(status.field)) {
		++data[status.field[p]].numberOfTiles;
	}

		boost::random::mt19937 rng{seed};
		boost::random::uniform_int_distribution<std::size_t> randomColumn{0, status.field.width() - 1};
		boost::random::uniform_int_distribution<std::size_t> randomRow{0, status.field.height() - 1};
	for (int i = 0; i < 100; ++i) {

		std::vector<std::pair<Point, std::size_t>> startingPoints;
		Status result = status;
		result.field.fill(-1);
		for (int i = 0; i < 3; ++i) {
			do {
				data[i].referencePoint = Point{static_cast<int>(randomColumn(rng)),
					static_cast<int>(randomRow(rng))};
			} while (result.field[data[i].referencePoint] != -1);

			result.field[data[i].referencePoint] = i;
			startingPoints.emplace_back(data[i].referencePoint, data[i].numberOfTiles);
		}
		int tries = 1000;
		auto field = result.field;
		bool ok = false;

		while (tries > 0 && !(ok = grow(field, startingPoints, rng))) {
			--tries;
			field = result.field;
		}

		if (ok) {
			result.field = field;
			return result;
		}
	}

	throw std::logic_error{"did not succeed"};
}

