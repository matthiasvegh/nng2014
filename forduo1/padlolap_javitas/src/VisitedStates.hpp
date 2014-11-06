#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "Status.hpp"
#include <unordered_map>

class VisitedStates {
	typedef std::unordered_map<Status, int> VisitedStateSet;
	VisitedStateSet visitedStates;
public:
	VisitedStates() = default;
	bool checkAndPush(const Status &elem, int heur);
	std::size_t size() const
	{
		return visitedStates.size();
	}
};

#endif /* VISITEDSTATES_H_ */
