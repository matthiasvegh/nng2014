#include "StatusCreator.hpp"
#include "Status.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>


Status
createStatus(int width, int height, const std::vector<std::string>& lines)
{
	Status result;
	result.field.reset(width, height, -1);
	if (height != static_cast<int>(lines.size())) {
		throw std::logic_error{"Bad input size"};
	}
	Point p;
	for (p.y = 0; p.y < height; ++p.y) {
		std::istringstream ss{lines[p.y]};
		for (p.x = 0; p.x < width; ++p.x) {
			ss >> result.field[p];
		}
	}
	return result;
}


Status loadStatusFromFile(const char *filename) {
	using namespace std;

	ifstream file(filename, ifstream::in);
	size_t height, width;
	file >> height >> width;
	string line;
	getline(file, line); // dummy
	std::vector<std::string> lines;
	lines.reserve(height);
	while (file.good())
	{
		getline(file, line);
		if (!line.empty()) {
			lines.push_back(std::move(line));
		}
	}
	return createStatus(width, height, lines);
}


