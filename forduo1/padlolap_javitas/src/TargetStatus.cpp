#include "TargetStatus.hpp"
#include "floodFill.hpp"
#include "Grow.hpp"
#include "DumperFunctions.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>

namespace {

struct ColorData {
	std::size_t numberOfTiles = 0;
	Point referencePoint;
};

}

Status findTargetStatus(const Status& status)
{
	ColorData data[3];
	Status result = status;
	result.field.fill(-1);
	for (Point p: arrayRange(status.field)) {
		++data[status.field[p]].numberOfTiles;
	}

	data[0].referencePoint = Point{0, 0};
	data[1].referencePoint = Point{static_cast<int>(status.field.width()) - 1, 0};
	data[2].referencePoint = Point{0, static_cast<int>(status.field.height()) - 1};

	for (int i = 0; i < 3; ++i) {
		result.field[data[i].referencePoint] = i;
		grow(result.field, data[i].referencePoint, data[i].numberOfTiles, [&result](Point p)
			{
				return result.field[p] < 0;
			});
	}

	return result;
}

