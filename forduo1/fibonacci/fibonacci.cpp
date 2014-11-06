#include <iostream>
#include <string>
#include <boost/version.hpp>
#include <boost/optional.hpp>

struct Node;
using NodePtr = Node*;
template <typename T>
using Optional = boost::optional<T>;

struct Node {
	NodePtr nMinus1 = nullptr; // the previous element of the fib sequence
	NodePtr nMinus2 = nullptr; // the prev element of the previous element
	Optional<std::string> value;
	std::size_t size = 0;
};

Node* addNode(Node& nodeMinus1, Node& nodeMinus2) {
	Node* newNode = new Node;
	newNode->nMinus1 = &nodeMinus1;
	newNode->nMinus2 = &nodeMinus2;
	newNode->size = newNode->nMinus1->size + newNode->nMinus2->size;
	return newNode;
}

struct Result { Node* nodeMinus1; Node* node; };
Result getNodeBiggerThan(std::size_t maxSize) {
	Node* node1 = new Node;
	Node* node2 = new Node;
	node1->value = "nagy";
	node1->size = node1->value->size();
	node2->value = "arpi";
	node2->size = node2->value->size();

	Node* nodeMinus1 = node2;
	Node* nodeMinus2 = node1;
	Node* newNode = addNode(*nodeMinus1, *nodeMinus2);
	std::cerr << newNode->size << std::endl;

	while (newNode->size < maxSize) {
		nodeMinus2 = nodeMinus1;
		nodeMinus1 = newNode;
		newNode = addNode(*nodeMinus1, *nodeMinus2);
		std::cerr << newNode->size << std::endl;
	}
	return {nodeMinus1, newNode};
}

struct CharResult { std::string str; std::size_t index; };
CharResult FindNth(Node* startNode, std::size_t n) {
	return CharResult{};
}

int main() {
	Result r = getNodeBiggerThan(54);
	//std::cout << r.node->size << std::endl;
}


