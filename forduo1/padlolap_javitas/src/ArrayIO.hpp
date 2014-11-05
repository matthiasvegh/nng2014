#include "Array.hpp"
#include "DumperFunctions.hpp"
#include <ostream>

template<typename T>
std::ostream& operator<<(std::ostream& os, const Array<T>& array)
{
	dumpArray(os, array, " ");
	return os;
}




