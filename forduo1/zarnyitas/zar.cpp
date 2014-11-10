#include <limits>
#include <cmath>
#include <vector>
#include <list>
#include <iostream>
#include <string.h> /* strcat */
#include <boost/optional.hpp>
#include <boost/multiprecision/cpp_int.hpp>

using namespace boost::multiprecision::literals;
namespace mp = boost::multiprecision;
using Num = mp::uint512_t;
using Rep = std::uint64_t;
using Opt = boost::optional<Num>;

// 0 -> +   1 -> *
const char* byte_to_binary(Num x, std::size_t size) {
	static char b[65];
	b[0] = '\0';

	std::int64_t z;
	for (z = std::pow(2, size - 2); z > 0; z >>= 1) {
		strcat(b, ((x & z) == z) ? "*" : "+");
	}

	return b;
}

Num calculate(Rep state, std::list<Num> v /*copy*/, Num expected) {
	assert(v.size() > 1);
	Num result = 0;
	auto it = v.begin();
	for (Rep z = std::pow(2, v.size() - 2); z > 0; z >>= 1) {
		if ((state & z) == z) { // nth bit set
			// result +
			++it;
		} else { // *
			auto itTmp = it;
			Num subResult = *it * *(++itTmp);
			if (subResult > expected) {
				return subResult;
			}
			*it = subResult;
			v.erase(itTmp);
		}
	}
	for (auto value : v) {
		result += value;
		if (result > expected) {
			return result;
		}
	}
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
		auto r = calculate(~node, {v.begin(), v.end()}, result);
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
	auto c = calculate(0b1, {1, 1}, 2);
	assert(c == 2);
}
void test_calc_2() {
	auto c = calculate(0b0, {1, 1}, 2);
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
	auto c = calculate(0b11, {1, 2, 3}, 100);
	assert(c == 6);
}
void test_calc_6() {
	auto c = calculate(0b10101, {1, 2, 3, 4, 5, 1}, 100);
	assert(c == 28);
}
void test_calc_7() {
	auto c = calculate(0b1010, {1, 2, 3, 4, 5}, 100);
	assert(c == 27);
}

int main() {
	// std::cout << byte_to_binary(7) << std::endl;

	test_calc_1();
	test_calc_2();
	test_calc_3();
	test_calc_4();
	test_calc_6();
	test_calc_7();
	//F{{2456, 5488, 3762, 6079, 6295, 748, 230}, 51921635}();
	 //F{{4816, 6427, 1591, 6385, 8694, 5280, 7439, 6747, 733, 7607, 5952, 3082,
	 //407, 8669, 4780, 7217, 8801, 7170, 9219, 4010, 8029, 4502},
	 //numAtoi("2279758461274168336368658723528310")}();
	 F{{8621, 9650, 9057, 5734, 7167, 2190, 8333, 5778, 2921, 728, 8554, 9900,
	 976, 8026, 4163, 1592, 3009, 7711, 8741, 537, 6184, 544, 7213, 3376,
	 8821, 9809, 5104, 9986, 3166, 862, 6038, 410, 2054, 4138, 6144, 1646,
	 520, 8691, 3207, 9591},
	 numAtoi("78822518924200536562972914309")}();

	return 0;
}
