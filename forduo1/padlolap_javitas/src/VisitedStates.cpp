#include "VisitedStates.hpp"
#include <boost/foreach.hpp>

bool VisitedStates::checkAndPush(const Status &elem)
{
	auto it = visitedStates.find(elem);
	if (it == visitedStates.end()) {
		visitedStates.insert(elem);
		return true;
	}
	return false;
}

