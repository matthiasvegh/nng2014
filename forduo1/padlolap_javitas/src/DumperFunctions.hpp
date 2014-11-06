#ifndef DUMPERFUNCTIONS_H_
#define DUMPERFUNCTIONS_H_

#include "Array.hpp"
#include "Status.hpp"
#include "Node.hpp"
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

template<class T>
void dumpArray(std::ostream &file, const Array<T> table,
		const std::string &title = "", int indent = 0) {
	std::string indentString(indent, ' ');
	if (!title.empty()) {
		file << indentString << title << std::endl;
	}
	Array<std::string> txts(table.width(), table.height());
	size_t maxlen = 0;
	for (Point  p: arrayRange(table)) {
		txts[p] = boost::lexical_cast<std::string>(table[p]);
		maxlen = std::max(maxlen, txts[p].size());
	}
	// leave a space between characters
	++maxlen;
	Point p;
	for (p.y = 0; p.y < static_cast<int>(table.height()); p.y++) {
		file << indentString;
		for (p.x = 0; p.x < static_cast<int>(table.width()); p.x++) {
			file.width(maxlen);
			file << txts[p];
		}
		file << std::endl;
	}
	file << std::endl;
}

inline
void dumpStatus(std::ostream &file, const Status &status,
		const std::string& title = "", int indent = 0)
{
	dumpArray(file, status.field, title, indent);
}

void dumpNode(std::ostream &file, const Node &node,
		std::string title = "", int indent = 0)
{
	std::ostringstream ss{title};
	ss << "{" << node.moveDescriptor.p1 << ", " << node.moveDescriptor.p2 << "}: " <<
		node.cost << " + " << node.heur << " = " << node.costFgv();

	dumpStatus(file, node.status, ss.str(), indent);
}




#endif /* DUMPERFUNCTIONS_H_ */
