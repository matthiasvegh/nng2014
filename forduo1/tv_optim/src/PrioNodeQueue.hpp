#ifndef PRIONODEQUEUE_H
#define PRIONODEQUEUE_H

#include "Node.hpp"
#include <queue>

struct NodeCompare {
	bool operator()(const Node::Ptr& lhs, const Node::Ptr& rhs)
	{
		auto costFgv1 = lhs->costFgv();
		auto costFgv2 = rhs->costFgv();

		if (costFgv1 == costFgv2) {
			return lhs->time < rhs->time;
		}
		return costFgv1 > costFgv2;
	}
};

class PrioNodeQueue {
	std::priority_queue<Node::Ptr, std::vector<Node::Ptr>, NodeCompare> queue;
public:
	void push(const Node::Ptr &node)
	{
		queue.push(node);
	}
	Node::Ptr pop() {
		if (queue.empty())
			return Node::Ptr();
		Node::Ptr result = queue.top();
		queue.pop();
		return result;
	}
	Node::Ptr peek() const
	{
		if (queue.empty())
			return Node::Ptr();
		Node::Ptr result = queue.top();
		return result;
	}
	size_t size() const
	{
		return queue.size();
	}
};


#endif /* PRIONODEQUEUE_H */
