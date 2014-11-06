#ifndef SOLVER_H
#define SOLVER_H

#include <deque>
#include <memory>

class Status;
class Node;

class Solver {
public:
	std::deque<std::shared_ptr<Node>> solve(Status status) const;
};

#endif /* SOLVER_H */
