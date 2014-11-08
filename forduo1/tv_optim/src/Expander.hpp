#ifndef SRC_EXPANDER_HPP
#define SRC_EXPANDER_HPP

#include "HeurCalculator.hpp"
#include "NodeFactory.hpp"
#include "PrioNodeQueue.hpp"
#include "VisitedStates.hpp"

class Expander {
	friend class InternalExpander;

	PrioNodeQueue& queue;
	VisitedStates& visitedStates;
	HeurCalculator calculator;
	NodeFactory nodeFactory;
	int expandedNodes = 0;
public:
	Expander(PrioNodeQueue& queue, VisitedStates& visitedStates):
		queue(queue),
		visitedStates(visitedStates),
		nodeFactory{calculator}
	{}
	~Expander();
	void expand(const Status::ConstPtr& status, std::shared_ptr<Node> base);
	int getExpandedNodes() const { return expandedNodes; }
};

#endif /* SRC_EXPANDER_HPP */
