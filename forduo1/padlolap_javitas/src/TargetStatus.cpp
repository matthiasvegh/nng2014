#include "TargetStatus.hpp"
#include "floodFill.hpp"
#include "Grow.hpp"
#include "DumperFunctions.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <stdexcept>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/random_device.hpp>

namespace {

struct ColorData {
	std::size_t numberOfTiles = 0;
	Point referencePoint;
};

}

Status findTargetStatus(const Status& status, boost::optional<unsigned> seed)
{
	ColorData data[3];
	for (Point p: arrayRange(status.field)) {
		++data[status.field[p]].numberOfTiles;
	}

	boost::random::mt19937 rng;

	if (seed) {
		rng.seed(*seed);
	} else {
		boost::random::random_device rnd;
		rng.seed(rnd);
	}

	boost::random::uniform_int_distribution<std::size_t> randomColumn{0, status.field.width() - 1};
	boost::random::uniform_int_distribution<std::size_t> randomRow{0, status.field.height() - 1};

	for (int outerTries = 0; outerTries < 20; ++outerTries) {

		std::vector<std::pair<Point, std::size_t>> startingPoints;
		Status result = status;

		for (int i = 0; i < 3; ++i) {
			do {
				data[i].referencePoint = Point{static_cast<int>(randomColumn(rng)),
					static_cast<int>(randomRow(rng))};
			} while (result.field[data[i].referencePoint] != i);

			result.field[data[i].referencePoint] = i;
			startingPoints.emplace_back(data[i].referencePoint, data[i].numberOfTiles);
		}

		for (int innerTries = 0; innerTries < 1000; ++innerTries) {
			auto field = result.field;

			if (grow(field, startingPoints, rng)) {
				std::cerr << "Tries: " << outerTries << " : " << innerTries << '\n';
				if (getNumberOfPartitions(field) != 3) {
					throw std::logic_error{"Bad number of partitions"};
				}

				std::size_t n[] = {0, 0, 0};
				for (Point p: arrayRange(field)) {
					++n[field[p]];
				}

				for (int i = 0; i < 3; ++i) {
					std::cerr << i << ": " << n[i] << " -> " << data[i].numberOfTiles <<
						std::endl;
					if (n[i] != data[i].numberOfTiles) {
						throw std::logic_error{"Bad tile number"};
					}
				}

				result.field = field;
				return result;
			}
		}
	}

	throw std::logic_error{"did not succeed"};
}

