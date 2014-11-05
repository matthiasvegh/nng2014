#include "StatusCreator.hpp"
#include "DumperFunctions.hpp"
#include "HeurCalculator.hpp"
#include <stdexcept>

int main(int argc, char* argv[])
{
	if (argc != 2) {
		throw std::logic_error{"Bad parameter"};
	}
	Status status = loadStatusFromFile(argv[1]);
	dumpStatus(std::cout, status);

	HeurCalculator heurCalculator;
	std::cout << "heur = " << heurCalculator.calculateStatus(status) << std::endl;
}
