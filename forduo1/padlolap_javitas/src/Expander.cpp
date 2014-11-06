#include "Expander.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include <iostream>

class InternalExpander {
	Status status;
	Node::Ptr base;
	Expander& owner;
	int heur;
public:
	InternalExpander(Status status, std::shared_ptr<Node> base, Expander &owner):
				status(std::move(status)),
				base(std::move(base)),
				owner(owner),
				heur(this->base ? this->base->heur :
						owner.calculator.calculateStatus(this->status))
	{
	}

	void expand();
	void expandNode(Point p, Point d);
};

void InternalExpander::expandNode(Point p1, Point p2)
{
	Status newStatus = status;
	std::swap(newStatus.field[p1], newStatus.field[p2]);
	Node::Ptr node =
			owner.nodeFactory.createNode(newStatus, MoveDescriptor(p1, p2), base);
	if (node->heur > heur) {
		return;
	}
	if (!owner.visitedStates.checkAndPush(newStatus, node->heur)) {
		return;
	}
	owner.queue.push(node);
	++owner.expandedNodes;
}

void InternalExpander::expand()
{
	if (owner.visitedStates.size() == 0) {
		owner.visitedStates.checkAndPush(status, heur);
	}
	auto range = arrayRange(status.field);
	for (auto it1 = range.begin(); it1 != range.end(); ++it1) {
		for (auto it2 = std::next(it1); it2 != range.end(); ++it2)
		{
			expandNode(*it1, *it2);
		}
	}
}

void Expander::expand(const Status& status, std::shared_ptr<Node> base)
{
	InternalExpander exp(status, std::move(base), *this);
	exp.expand();
}

Expander::~Expander()
{
	std::cerr << "Expanded nodes: " << expandedNodes << std::endl;
}


