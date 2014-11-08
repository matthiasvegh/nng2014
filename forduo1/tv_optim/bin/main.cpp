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
		Channel* previousChannel = nullptr;

		for (const auto& node: result) {
			auto channel = node->status->program->channel;
			if (channel != previousChannel && !channel->name.empty()) {
				std::cout << time << " " << channel->name << std::endl;
				previousChannel = channel;
			}
			time = node->status->program->endTime;
		}

	}
}
