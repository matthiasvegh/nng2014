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
	std::size_t start = 0;
};

Node* addNode(Node* nodeMinus1, Node* nodeMinus2) {
	Node* newNode = new Node;
	newNode->nMinus1 = nodeMinus1;
	newNode->nMinus2 = nodeMinus2;
	newNode->size = newNode->nMinus1->size + newNode->nMinus2->size;
	newNode->start = newNode->nMinus1->start + newNode->nMinus1->size;
	//std::cerr << "pos: " << newNode->start << std::endl;
	//std::cerr << "size: " << newNode->size << std::endl;
	return newNode;
}

struct Result {
	Node* node;
};
Result getNode(const std::string& str1, const std::string& str2,
						 std::size_t position) {
	Node* node1 = new Node;
	Node* node2 = new Node;
	node1->value = str1;
	node1->size = node1->value->size();
	node1->start = 0;
	//std::cerr << "pos: " << node1->start << std::endl;
	//std::cerr << "size: " << node1->size << std::endl;
	if (node1->start + node1->size > position)
		return {node1};

	node2->value = str2;
	node2->size = node2->value->size();
	node2->start = node1->start + node1->size;
	//std::cerr << "pos: " << node2->start << std::endl;
	//std::cerr << "size: " << node2->size << std::endl;
	if (node2->start + node2->size > position)
		return {node2};


	Node* nodeMinus1 = node2;
	Node* nodeMinus2 = node1;
	Node* newNode = addNode(nodeMinus1, nodeMinus2);

	while (newNode->start + newNode->size <= position) {
		nodeMinus2 = nodeMinus1;
		nodeMinus1 = newNode;
		newNode = addNode(nodeMinus1, nodeMinus2);
	}
	return {newNode};
}

struct CharResult {
	std::string str;
	std::size_t index;
};
CharResult findNth(Node* startNode, std::size_t index) {
	if(startNode->nMinus1 == nullptr) {
		return CharResult{startNode->value.get(), index};
	}
	if (index < startNode->nMinus2->size) {
		return findNth(startNode->nMinus2, index);
	} else {
		return findNth(startNode->nMinus1, index - startNode->nMinus2->size);
	}
	return CharResult{};
}

CharResult fibonacci(const std::string& str1, const std::string& str2,
					 std::size_t N) {
	assert(N >= 1);
	N = N - 1;
	Result r = getNode(str1, str2, N);
	auto cr = findNth(r.node, N - r.node->start);
	return cr;
	// std::cout << cr.str << " " << cr.index << std::endl;
}

void test1() {
	auto r = fibonacci("alma", "korte", 17);
	assert(r.str == "korte");
	assert(r.index == 3);
}
void test2() {
	auto r = fibonacci("alma", "korte", 7);
	assert(r.str == "korte");
	assert(r.index == 2);
}
void test3() {
	auto r = fibonacci("alma", "korte", 3);
	assert(r.str == "alma");
	assert(r.index == 2);
}
void test4() {
	auto r = fibonacci("a", "b", 16);
	assert(r.str == "a");
	assert(r.index == 0);
}

void testA0() {
	auto r = fibonacci("a", "b", 1);
	assert(r.str == "a");
	assert(r.index == 0);
}
void testB0() {
	auto r = fibonacci("a", "b", 2);
	assert(r.str == "b");
	assert(r.index == 0);
}
void testA() {
	auto r = fibonacci("a", "b", 3);
	assert(r.str == "a");
	assert(r.index == 0);
}
void testB() {
	auto r = fibonacci("a", "b", 4);
	assert(r.str == "b");
	assert(r.index == 0);
}
void testC() {
	auto r = fibonacci("a", "b", 5);
	assert(r.str == "b");
	assert(r.index == 0);
}
void testD() {
	auto r = fibonacci("a", "b", 10);
	assert(r.str == "b");
	assert(r.index == 0);
}

int main() {
	testA0();
	testB0();
	testA();
	testB();
	testC();
	testD();
	//test0();
	test1();
	test2();
	test3();
	test4();

	auto r = fibonacci("nagy", "arpi", 54);
	std::cout << "nagy" << "arpi:" << 54 << " " << r.str[r.index] << std::endl;
	std::cout << r.str << " " << r.index << std::endl;
}
