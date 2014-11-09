#ifndef SIMPLENODE_HPP
#define SIMPLENODE_HPP

#include "Status.hpp"
#include "MoveDescriptor.hpp"

struct SimpleNode {
	Status status;
	MoveDescriptor moveDescriptor;

	SimpleNode(Status status, MoveDescriptor moveDescriptor):
		status(std::move(status)),
		moveDescriptor(std::move(moveDescriptor))
	{}

};


#endif /* SIMPLENODE_HPP */
