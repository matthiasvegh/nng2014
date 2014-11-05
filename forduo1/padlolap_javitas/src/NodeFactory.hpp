#ifndef NODEFACTORY_HPP
#define NODEFACTORY_HPP

#include "Node.hpp"
#include "HeurCalculator.hpp"
#include "Status/Status.hpp"
#include <memory>

class NodeFactory {
	int numNodes;
	HeurCalculator::Ptr calculator;
public:
	typedef std::shared_ptr<NodeFactory> Ptr;

	NodeFactory(
			const HeurCalculator::Ptr& calculator,
			const HeurCalculator::Ptr& experimentalCalculator = HeurCalculator::Ptr()):
		numNodes(0),
		calculator(calculator),
		experimentalCalculator(experimentalCalculator)
	{}
	Node::Ptr createNode(
			const Status & status,
			const MoveDescriptor& moveDescriptor,
			const Node::Ptr& ancestor)
	{
		return std::make_shared<Node>(
				status.state(), moveDescriptor, ancestor, 6,
				calculator->calculateStatus(status),
				++numNodes);
	}
};


#endif /* NODEFACTORY_HPP */
