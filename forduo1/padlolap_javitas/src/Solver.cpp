#include "Solver.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include "Expander.hpp"
#include <boost/lexical_cast.hpp>

class InternalSolver {
	VisitedStates visitedStates;
	PrioNodeQueue queue;
	Expander expander{visitedStates, queue};
	Node::Ptr currentNode;
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

