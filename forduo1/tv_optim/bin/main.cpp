#include "ProgramGraph.hpp"
#include "Solver.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		throw std::logic_error{"Bad parameter"};
	}

	auto programGraph = loadFromFile(argv[1]);

	Status status;
	status.program = programGraph.beginProgram.get();
	Status targetStatus;
	targetStatus.program = programGraph.endProgram.get();

	Solver solver;
	auto result = solver.solve(std::move(status), std::move(targetStatus));

	if (result.empty()) {
		std::cout << "No solution." << std::endl;
	} else {
		int time = 0;

		for (const auto& node: result) {
			std::cout << time << " " << node->status->program->channel->name << std::endl;
			time = node->status->program->endTime;
		}

	}
}
