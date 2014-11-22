#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "TargetStatus.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

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

	Status target = findTargetStatus(status, seed);
	std::cout << target.height() << " " << target.width() << std::endl;
	dumpStatus(std::cout, target);
}
