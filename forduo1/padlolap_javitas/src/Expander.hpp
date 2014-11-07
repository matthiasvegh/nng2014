#ifndef SRC_EXPANDER_HPP
#define SRC_EXPANDER_HPP

#include "HeurCalculator.hpp"
#include "NodeFactory.hpp"
#include "VisitedStates.hpp"
#include "PrioNodeQueue.hpp"

class Expander {
	friend class InternalExpander;

	VisitedStates& visitedStates;
	PrioNodeQueue& queue;
	HeurCalculator calculator;
	NodeFactory nodeFactory;
	int expandedNodes = 0;
	Status targetStatus;
public:
	Expander(VisitedStates& vs, PrioNodeQueue& queue, Status targetStatus):
		visitedStates(vs), queue(queue),
		calculator{targetStatus},
		nodeFactory{calculator},
		targetStatus(std::move(targetStatus))
	{}
	~Expander();
	void expand(const Status::ConstPtr& status, std::shared_ptr<Node> base);
	int getExpandedNodes() const { return expandedNodes; }
};

#endif /* SRC_EXPANDER_HPP */
