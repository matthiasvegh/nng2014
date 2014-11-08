#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "TargetStatus.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		throw std::logic_error{"Bad parameter"};
	}
	Status status = loadStatusFromFile(argv[1]);
	unsigned seed = boost::lexical_cast<unsigned>(argv[2]);

	Status target = findTargetStatus(status, seed);
	std::cout << target.field.height() << " " << target.field.width() << std::endl;
	dumpStatus(std::cout, target);
}
