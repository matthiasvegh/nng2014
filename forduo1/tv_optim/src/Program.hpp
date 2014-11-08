#ifndef SRC_PROGRAM_HPP
#define SRC_PROGRAM_HPP

#include "Edge.hpp"
#include <vector>

struct Channel;

struct Program {
	int startTime = 0;
	int endTime = 0;
	Channel* channel = nullptr;
	std::vector<Edge> next;

	Program() = default;
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;
	Program(Program&&) = default;
	Program& operator=(Program&&) = default;
};


#endif /* SRC_PROGRAM_HPP */
