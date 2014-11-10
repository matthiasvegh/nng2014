#include <limits>
#include <cmath>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/algorithm/string/trim.hpp>

using namespace boost::multiprecision::literals;
namespace mp = boost::multiprecision;
using Num = mp::uint512_t;
using Rep = std::uint64_t;

// 0 -> +   1 -> *
std::string byte_to_binary(Num x, std::size_t size) {
	std::string result;
	for (int i = 0; i < size - 1; ++i) {
		result += (x & (1 << i)) ? '*' : '+';
	}
	return result;
}

Num calculate(Rep state, const std::vector<Num>& v, const Num& expected) {
	Num result = 0;
	Num subResult = 1;
	for (int i = 0; i < v.size() - 1; ++i) {
		//std::cerr << v[i];
		subResult *= v[i];

		if (! (state & (1 << i))) {
			// +
			//std::cerr << "+";
			result += subResult;
			subResult = 1;
		} else {
			//std::cerr << "*";
		}
	}
	result += subResult * v.back();
	//std::cerr << v.back() << "=" << result << "\n";
	return result;
}

struct F {
	std::vector<Num> v;
	Num result;
	const std::size_t numberOfBits = v.size() - 1;
	const Rep size = std::pow(2, numberOfBits);
	const Rep hundredth = size / 100;
	Rep solved = 0;

	F(const std::vector<Num>& v, Num result):
			v(v), result(result)
	{}

	bool solve(Rep node, int firstBit, const std::string& indent) {
		if (solved % hundredth == 0) {
			std::cerr << solved / hundredth << "%\r";
		}

		//std::cerr << indent << "-> " << byte_to_binary(node, v.size()) << '\n';
		auto r = calculate(node, {v.begin(), v.end()}, result);
		//std::cerr << "-> " << byte_to_binary(node, v.size()) << "=" << r << '\n';
		if (r == result) {
			std::cout << byte_to_binary(node, v.size()) << '\n';
			return true;
		} else if (r > result) {
			//std::cerr << indent << "-> cut\n";
			solved += std::pow(2, numberOfBits - firstBit);
			return false;
		} else {
			++solved;

			for (int bit = firstBit; bit < numberOfBits; ++bit) {
				if (solve(node | (1 << bit), bit + 1, indent + ' ')) {
					return true;
				}
			}

			return false;
		}

	}

	void operator()() {
		solve(0, 0, "");
	}
};

Num numAtoi(const char* const str) {
	Num res = 0; // Initialize result
	for (int i = 0; str[i] != '\0'; ++i) res = res * 10 + str[i] - '0';
	return res;
}

void test_calc_1() {
	auto c = calculate(0b0, {1, 1}, 2);
	assert(c == 2);
}
void test_calc_2() {
	auto c = calculate(0b1, {1, 1}, 2);
	assert(c == 1);
}
void test_calc_3() {
	auto c = calculate(0b10, {1, 2, 3}, 100);
	assert(c == 7);
}
void test_calc_4() {
	auto c = calculate(0b01, {1, 2, 3}, 100);
	assert(c == 5);
}
void test_calc_5() {
	auto c = calculate(0b00, {1, 2, 3}, 100);
	assert(c == 6);
}
void test_calc_6() {
	auto c = calculate(0b01010, {1, 2, 3, 4, 5, 1}, 100);
	assert(c == 28);
}
void test_calc_7() {
	auto c = calculate(0b1010, {1, 2, 3, 4, 5}, 100);
	assert(c == 27);
}

int main(int argc, char* argv[]) {
	// std::cout << byte_to_binary(7) << std::endl;

	test_calc_1();
	test_calc_2();
	test_calc_3();
	test_calc_4();
	test_calc_6();
	test_calc_7();

	std::ifstream file{argv[1]};

	std::vector<Num> values;
	while (file.good()) {
		std::string s;
		file >> s;
		boost::algorithm::trim(s);
		if (!s.empty()) {
			values.push_back(numAtoi(s.c_str()));
		}
	}
	Num result = values.back();
	values.pop_back();
	for (const auto& v: values) {
		std::cerr << v << " ";
	}
	std::cerr << "\n" << result << std::endl;
	F{values, result}();

	return 0;
}
