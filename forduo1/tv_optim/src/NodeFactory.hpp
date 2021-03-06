#ifndef SRC_NODEFACTORY_HPP
#define SRC_NODEFACTORY_HPP

#include "Node.hpp"
#include "HeurCalculator.hpp"
#include "Status.hpp"
#include <memory>

class NodeFactory {
	int numNodes;
	HeurCalculator& calculator;
public:
	typedef std::shared_ptr<NodeFactory> Ptr;

	NodeFactory(HeurCalculator& calculator):
		numNodes(0),
		calculator(calculator)
	{}
	Node::Ptr createNode(
			const Status::ConstPtr& status,
			const Node::Ptr& ancestor)
	{
		int previousEndTime = ancestor ? ancestor->status->program->endTime : 0;
		return std::make_shared<Node>(
				status, ancestor, status->program->startTime - previousEndTime,
				calculator.calculateStatus(*status),
				++numNodes);
	}
};


#endif /* SRC_NODEFACTORY_HPP */
