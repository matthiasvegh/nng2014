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
	int expandedNodes;
public:
	Expander(VisitedStates& vs, PrioNodeQueue& queue):
		visitedStates(vs), queue(queue),
		nodeFactory{calculator}
	{}
	void expand(std::shared_ptr<Node> base);
	int getExpandedNodes() const { return expandedNodes; }
};

#endif /* SRC_EXPANDER_HPP */
