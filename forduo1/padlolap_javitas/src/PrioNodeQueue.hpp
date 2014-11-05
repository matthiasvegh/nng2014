#ifndef PRIONODEQUEUE_H
#define PRIONODEQUEUE_H

#include "Node.hpp"
#include <queue>

struct NodeCompare {
	bool operator()(const Node::Ptr& lhs, const Node::Ptr& rhs)
	{
		return lhs->costFgv() > rhs->costFgv();
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
