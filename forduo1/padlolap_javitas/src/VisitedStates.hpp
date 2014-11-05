#ifndef VISITEDSTATES_H_
#define VISITEDSTATES_H_

#include "Status.hpp"
#include <unordered_set>

class VisitedStates {
	typedef std::unordered_set<Status> VisitedStateSet;
	VisitedStateSet visitedStates;
public:
	VisitedStates() = default;
	bool checkAndPush(const Status &elem);
	std::size_t size() const
	{
		return visitedStates.size();
	}
};

#endif /* VISITEDSTATES_H_ */
