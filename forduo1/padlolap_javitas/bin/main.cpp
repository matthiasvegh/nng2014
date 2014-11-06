#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "Solver.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		throw std::logic_error{"Bad parameter"};
	}
	Status status = loadStatusFromFile(argv[1]);
	Solver solver;
	auto result = solver.solve(status);

	if (result.empty()) {
		std::cout << "No solution." << std::endl;
	} else {
		std::ofstream file{"solution.dump", std::ios::out | std::ios::trunc};
		std::cout << result.size() << "\n";
		for (const auto& node: result) {
			const auto& moveDescriptor = node->moveDescriptor;
			std::cout << moveDescriptor.p1.y << " " << moveDescriptor.p1.x << " " <<
				moveDescriptor.p2.y << " " << moveDescriptor.p2.x << "\n";
			dumpNode(file, *node);
		}
		std::cout << std::endl;

	}
}
