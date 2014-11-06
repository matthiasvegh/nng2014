#ifndef NODE_H
#define NODE_H

#include <memory>
#include <boost/noncopyable.hpp>
#include <deque>
#include "Status.hpp"
#include "MoveDescriptor.hpp"

struct Node {
	typedef std::shared_ptr<Node> Ptr;

	Status::ConstPtr status;
	MoveDescriptor moveDescriptor;
	Ptr ancestor;
	int cost;
	int heur;
	int time;

	Node(Status::ConstPtr status, const MoveDescriptor& moveDescriptor, Ptr ancestor,
			int c, int heur, int time):
				status(std::move(status)),
				moveDescriptor(moveDescriptor),
				ancestor(std::move(ancestor)),
				cost(this->ancestor ? this->ancestor->cost + c : c),
				heur(heur),
				time(time)
	{}
	Node(const Node&) = delete;
	Node& operator=(const Node&) = delete;
	Node(Node&&) = default;
	Node& operator=(Node&&) = default;

	int costFgv() const { return cost * !greedy + heur; }

	static bool greedy;
};

inline
std::deque<Node::Ptr> pathToBase(Node::Ptr node, const Node::Ptr& base)
{
	std::deque<Node::Ptr> result;
	while (node && node != base)
	{
		result.push_front(node);
		node = node->ancestor;
	}
	return result;
}

inline std::deque<Node::Ptr> pathToRoot(Node::Ptr node) {
	return pathToBase(node, Node::Ptr());
}

#endif /* NODE_H */
