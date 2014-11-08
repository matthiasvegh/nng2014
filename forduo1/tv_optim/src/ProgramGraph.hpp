#ifndef SRC_PROGRAMGRAPH_HPP
#define SRC_PROGRAMGRAPH_HPP

#include "Channel.hpp"

struct ProgramGraph {
	std::vector<std::unique_ptr<Channel>> channels;
	std::vector<Program*> programs;

	ProgramGraph() = default;
	ProgramGraph(const ProgramGraph&) = delete;
	ProgramGraph& operator=(const ProgramGraph&) = delete;
	ProgramGraph(ProgramGraph&&) = default;
	ProgramGraph& operator=(ProgramGraph&&) = default;
};

ProgramGraph loadFromFile(const char* fileName);

#endif /* SRC_PROGRAMGRAPH_HPP */
