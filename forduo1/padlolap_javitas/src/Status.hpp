#ifndef STATUS_HPP
#define STATUS_HPP

#include "Array.hpp"

using Value = int;
struct Status {
	Array<int> field;
};

inline Value getValue(const Status& status, Point p)
{
	return arrayAt(status.field, p, -1);
}

#endif /* STATUS_HPP */
