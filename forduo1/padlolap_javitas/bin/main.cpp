#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "HeurCalculator.hpp"
#include "Expander.hpp"
#include <stdexcept>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		throw std::logic_error{"Bad parameter"};
	}
	Status status = loadStatusFromFile(argv[1]);
	dumpStatus(std::cout, status);

	HeurCalculator heurCalculator;
	std::cout << "heur = " << heurCalculator.calculateStatus(status) << std::endl;

	VisitedStates visitedStates;
	PrioNodeQueue queue;
	Expander expander{visitedStates, queue};
	expander.expand(status, Node::Ptr{});

	while (queue.size() != 0){
		auto node = queue.pop();
		dumpNode(std::cout, *node);
	}
}
