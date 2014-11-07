#include "Solver.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include "Expander.hpp"
#include "DumperFunctions.hpp"
#include <iostream>

class InternalSolver {
	VisitedStates visitedStates;
	PrioNodeQueue queue;
	Expander expander;
	Status::ConstPtr status;
	Node::Ptr currentNode;
	unsigned iterations = 0;
public:
	InternalSolver(Status::ConstPtr status, Status targetStatus):
		expander{visitedStates, queue, std::move(targetStatus)},
		status(std::move(status))
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
				dumpStatus(std::cerr, *status);
			}
		} while (currentNode->heur > 0);
		std::deque<Node::Ptr> result;
		if (currentNode && currentNode->heur == 0) {
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

