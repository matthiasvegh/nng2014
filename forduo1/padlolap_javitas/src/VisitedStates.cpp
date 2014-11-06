#include "VisitedStates.hpp"
#include <boost/foreach.hpp>

bool VisitedStates::checkAndPush(const Status::ConstPtr& elem, int heur)
{
	auto it = visitedStates.find(elem);
	if (it == visitedStates.end()) {
		visitedStates.insert({elem, heur});
		return true;
	}

	if (it->second > heur) {
		it->second = heur;
		return true;
	}

	return false;
}

