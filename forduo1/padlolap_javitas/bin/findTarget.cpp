#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "TargetStatus.hpp"
#include <stdexcept>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		throw std::logic_error{"Bad parameter"};
	}
	Status status = loadStatusFromFile(argv[1]);

	Status target = findTargetStatus(status);
	std::cout << target.field.height() << " " << target.field.width() << std::endl;
	dumpStatus(std::cout, target);
}
