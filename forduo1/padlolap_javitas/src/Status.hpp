#ifndef STATUS_HPP
#define STATUS_HPP

#include "Array.hpp"

using Value = int;
using Status = Array<Value>;

inline Value value(const Status& status, Point p)
{
	return arrayAt(Status, p, -1);
}

#endif /* STATUS_HPP */
