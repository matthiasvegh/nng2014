#include "Expander.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include <iostream>

class InternalExpander {
	Status::ConstPtr status;
	Node::Ptr base;
	Expander& owner;
	int heur;
public:
	InternalExpander(Status::ConstPtr status, std::shared_ptr<Node> base, Expander &owner):
				status(std::move(status)),
				base(std::move(base)),
				owner(owner),
				heur(this->base ? this->base->heur :
						owner.calculator.calculateStatus(*this->status))
	{
	}

	void expand();
	void expandNode(Program* program);
};

void InternalExpander::expandNode(Program* program)
{
	auto newStatus = std::make_shared<Status>();
	newStatus->program = program;

	Node::Ptr node = owner.nodeFactory.createNode(newStatus, base);
	if (!owner.visitedStates.checkAndPush(newStatus, node->costFgv())) {
		return;
	}

	owner.queue.push(node);
	++owner.expandedNodes;
}

void InternalExpander::expand()
{
	for (const auto& next: status->program->next) {
		expandNode(next.program);
	}
}

void Expander::expand(const Status::ConstPtr& status, std::shared_ptr<Node> base)
{
	InternalExpander exp(status, std::move(base), *this);
	exp.expand();
}

Expander::~Expander()
{
	std::cerr << "Expanded nodes: " << expandedNodes << std::endl;
}


