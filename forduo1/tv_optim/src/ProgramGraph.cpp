#include "ProgramGraph.hpp"
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>

ProgramGraph loadFromFile(const char* filename)
{
	ProgramGraph result;

	std::ifstream file{filename};

	std::string line;
	std::vector<std::string> lines;
	while (file.good())
	{
		std::getline(file, line);
		if (!line.empty()) {
			lines.push_back(std::move(line));
		}
	}

	auto it = lines.begin();
	while (it != lines.end()) {
		result.channels.emplace_back(new Channel);
		Channel& channel = *result.channels.back();
		channel.name = *it++;

		int numPrograms = boost::lexical_cast<int>(*it++);
		channel.programs.reserve(numPrograms);

		for (int i = 0; i < numPrograms; ++i) {
			channel.programs.emplace_back(new Program);
			Program& program = *channel.programs.back();
			program.channel = &channel;

			std::istringstream ss{*it++};
			int length;
			ss >> program.startTime >> length;
			program.endTime = program.startTime + length;
			result.programs.push_back(&program);
		}
	}

	for (auto program1: result.programs) {
		for (auto program2: result.programs) {
			int distance = program2->startTime - program1->endTime;
			if (distance >= 0) {
				program1->next.emplace_back(program2, distance);
			}
		}
	}

	return result;
}

