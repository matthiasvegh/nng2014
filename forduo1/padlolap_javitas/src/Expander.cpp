#include "Expander.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include <iostream>

class InternalExpander {
	Node::Ptr base;
	Expander& owner;
public:
	InternalExpander(std::shared_ptr<Node> base, Expander &owner):
				base(std::move(base)),
				owner(owner)
	{
	}

	void expand();
	void expandNode(Point p, Point d);
};

void InternalExpander::expandNode(Point p1, Point p2)
{
	Status newStatus = base->status;
	std::swap(newStatus.field[p1], newStatus.field[p2]);
	Node::Ptr node =
			owner.nodeFactory.createNode(newStatus, MoveDescriptor(p1, p2), base);
	if (!owner.visitedStates.checkAndPush(newStatus, node->heur)) {
		return;
	}
	owner.queue.push(node);
	{
		if (++owner.expandedNodes % 10000 == 0)
			std::cerr << boost::format(
					"Expanded nodes: %d.\n"
					"Nodes in queue: %d.\n"
					"Cost function: %d\n") %
				owner.expandedNodes % owner.queue.size() %
				(node ? node->costFgv() : -1) << std::endl;
	}
}

void InternalExpander::expand()
{
	if (base && owner.visitedStates.size() == 0) {
		owner.visitedStates.checkAndPush(base->status, owner.calculator.calculateStatus(base->status));
	}
	auto range = arrayRange(base->status.field);
	for (auto it1 = range.begin(); it1 != range.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != range.end(); ++it2)
		{
			expandNode(*it1, *it2);
		}
	}
}

void Expander::expand(std::shared_ptr<Node> base)
{
	InternalExpander exp(std::move(base), *this);
	exp.expand();
}


