#ifndef SRC_EDGE_HPP
#define SRC_EDGE_HPP

struct Program;

struct Edge {
	Program* program;
	int distance;

	Edge(Program* program, int distance): program(program), distance(distance) {}
};


#endif /* SRC_EDGE_HPP */
