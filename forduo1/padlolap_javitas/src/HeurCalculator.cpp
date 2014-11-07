#include "HeurCalculator.hpp"
#include "Status.hpp"
#include "floodFill.hpp"
//#include "DumperFunctions.hpp"
//#include <iostream>

//namespace {

//int getNumberOfPartitions(const Status& status)
//{
	//int result = 0;
	//Array<bool> visited(status.field.width(), status.field.height(), false);

	//for (Point p: arrayRange(status.field)) {
		//if (!visited[p]) {
			//floodFill(status, p, visited);
			//++result;
		//}
	//}
	//return result;
//}

//}

int HeurCalculator::calculateStatus(const Status& status)
{
	int result = 0;
	for (Point p: arrayRange(status.field)) {
		if (arrayAt(targetStatus.field, p, -1) != status.field[p]) {
			++result;
		}
	}

	return result;
}


