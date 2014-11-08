#include "ProgramGraph.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		throw std::logic_error{"Bad parameter"};
	}

	auto programGraph = loadFromFile(argv[1]);

	std::cout << "digraph tv {\n";
	for (auto program1: programGraph.programs) {
		for (const auto& edge: program1->next) {
			auto program2 = edge.program;
			std::cout << "  " << program1->channel->name << program1->startTime << " -> " <<
				program2->channel->name << program2->startTime <<
				" [label=\"" << edge.distance << "\"]\n";
		}
	}
	std::cout << "}" << std::endl;
}
