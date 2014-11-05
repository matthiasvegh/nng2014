#ifndef STATUS_HPP
#define STATUS_HPP

#include "Array.hpp"

using Value = int;
struct Status {
	Array<int> field;

	Status() = default;
	Status(const Status&) = default;
	Status& operator=(const Status&) = default;
	Status(Status&&) = default;
	Status& operator=(Status&&) = default;
};

inline Value getValue(const Status& status, Point p)
{
	return arrayAt(status.field, p, -1);
}

#endif /* STATUS_HPP */
