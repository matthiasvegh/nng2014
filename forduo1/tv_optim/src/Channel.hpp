#ifndef SRC_CHANNEL_HPP
#define SRC_CHANNEL_HPP

#include "Program.hpp"
#include <vector>
#include <string>
#include <memory>

struct Channel {
	std::string name;
	std::vector<std::unique_ptr<Program>> programs;

	Channel() = default;
	Channel(const Channel&) = delete;
	Channel& operator=(const Channel&) = delete;
	Channel(Channel&&) = default;
	Channel& operator=(Channel&&) = default;
};


#endif /* SRC_CHANNEL_HPP */
