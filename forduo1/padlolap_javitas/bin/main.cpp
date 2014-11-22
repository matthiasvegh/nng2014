#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "fastSolve.hpp"
#include "TargetStatus.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 2) {
		throw std::logic_error{"Bad parameter"};
	}
	Status status = loadStatusFromFile(argv[1]);
	boost::optional<unsigned> seed;
	if (argc > 2) {
		seed = boost::lexical_cast<unsigned>(argv[2]);
	}

	Status targetStatus = findTargetStatus(status, seed);
	auto result = fastSolve(std::move(status), std::move(targetStatus));

	if (result.empty()) {
		std::cout << "No solution." << std::endl;
	} else {
		//std::ofstream file{"solution.dump", std::ios::out | std::ios::trunc};
		std::cout << result.size() << "\n";
		for (const auto& node: result) {
			const auto& moveDescriptor = node.moveDescriptor;
			std::cout << moveDescriptor.p1.y << " " << moveDescriptor.p1.x << " " <<
				moveDescriptor.p2.y << " " << moveDescriptor.p2.x << "\n";
			//dumpSimpleNode(file, node);
		}
		std::cout << std::endl;

	}
}
