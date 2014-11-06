#include "Solver.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include "Expander.hpp"
#include <iostream>

class InternalSolver {
	VisitedStates visitedStates;
	PrioNodeQueue queue;
	Expander expander{visitedStates, queue};
	Node::Ptr currentNode;
	unsigned iterations = 0;
public:
	bool expandSerial(Status& status)
	{
		expander.expand(status, currentNode);
		currentNode = queue.pop();
		if (!currentNode) {
			return false;
		}
		status = currentNode->status;
		return true;
	}

	std::deque<Node::Ptr> solve(Status& status) {
		currentNode.reset();
		do {
			if (!expandSerial(status)) {
				break;
			}

			if (++iterations % 1000 == 0) {
				std::cerr << boost::format(
						"Iterations: %d\n"
						"Expanded nodes: %d.\n"
						"Nodes in queue: %d.\n"
						"Cost function: %d\n") %
					iterations % expander.getExpandedNodes() % queue.size() %
					(currentNode ? currentNode->costFgv() : -1) << std::endl;
			}
		} while (currentNode->heur > 0);
		std::deque<Node::Ptr> result;
		if (currentNode && currentNode->heur == 0) {
			result = pathToRoot(currentNode);
		}
		return result;
	}

};

std::deque<Node::Ptr> Solver::solve(Status status) const
{
	InternalSolver solver;
	return solver.solve(status);

}

