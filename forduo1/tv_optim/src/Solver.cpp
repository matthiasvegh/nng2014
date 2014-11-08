#include "Solver.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include "Expander.hpp"
#include <boost/format.hpp>
#include <iostream>

class InternalSolver {
	PrioNodeQueue queue;
	VisitedStates visitedStates;
	Expander expander;
	Status::ConstPtr status;
	Status targetStatus;
	Node::Ptr currentNode;
	unsigned iterations = 0;
public:
	InternalSolver(Status::ConstPtr status, Status targetStatus):
		expander{queue, visitedStates},
		status(std::move(status)),
		targetStatus(std::move(targetStatus))
	{}

	bool expandSerial()
	{
		expander.expand(status, currentNode);
		currentNode = queue.pop();
		if (!currentNode) {
			return false;
		}
		status = currentNode->status;
		return true;
	}

	std::deque<Node::Ptr> solve() {
		currentNode.reset();
		do {
			if (!expandSerial()) {
				break;
			}

			if (++iterations % 10000 == 0) {
				std::cerr << boost::format(
						"Iterations: %d\n"
						"Expanded nodes: %d.\n"
						"Nodes in queue: %d.\n"
						"Cost function: %d\n") %
					iterations % expander.getExpandedNodes() % queue.size() %
					(currentNode ? currentNode->costFgv() : -1);
			}
		} while (*currentNode->status != targetStatus);
		std::deque<Node::Ptr> result;
		if (currentNode && *currentNode->status == targetStatus) {
			result = pathToRoot(currentNode);
		}
		return result;
	}

};

std::deque<Node::Ptr> Solver::solve(Status status, Status targetStatus) const
{
	InternalSolver solver(std::make_shared<Status>(std::move(status)),
			std::move(targetStatus));
	return solver.solve();

}

