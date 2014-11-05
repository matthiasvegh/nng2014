#ifndef SRC_MOVEDESCRIPTOR_HPP
#define SRC_MOVEDESCRIPTOR_HPP

#include "Point.hpp"

struct MoveDescriptor {
	Point p1;
	Point p2;

	MoveDescriptor(Point p1, Point p2):p1(p1), p2(p2) {}
};


#endif /* SRC_MOVEDESCRIPTOR_HPP */
