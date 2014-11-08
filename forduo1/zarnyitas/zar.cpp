#include <limits>
#include <cmath>
#include <vector>
#include <list>
#include <iostream>
#include <string.h> /* strcat */
#include <boost/optional.hpp>

using Num = std::int64_t;
using Rep = Num;
using Opt = boost::optional<Num>;

// 1 -> +   0 -> *
const char* byte_to_binary(Num x, std::size_t size) {
	static char b[65];
	b[0] = '\0';

	std::int64_t z;
	for (z = std::pow(2, size - 2); z > 0; z >>= 1) {
		strcat(b, ((x & z) == z) ? "+" : "*");
	}

	return b;
}


Opt calculate(Rep state, std::list<Num> v /*copy*/) {
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
			//if (subResult >  TODO return immediately
			*it = subResult;
			v.erase(itTmp);
		}
	}
	for (auto value : v) { result += value; }
	return result;
}

void f(const std::vector<Num>& v, Num result) {
	for (Rep i = 0; i < std::pow(2, v.size() - 1); ++i) {
		auto r = calculate(i, {v.begin(), v.end()});
		std::cout << *r << '\n';
		if (r && (*r == result)) {
			std::cout << byte_to_binary(i, v.size()) << '\n';
			break;
		}
	}
}

void test_calc_1() {
	auto c = calculate(0b1, {1,1});
	assert(*c == 2);
}
void test_calc_2() {
	auto c = calculate(0b0, {1,1});
	assert(*c == 1);
}
void test_calc_3() {
	auto c = calculate(0b10, {1,2,3});
	assert(*c == 7);
}
void test_calc_4() {
	auto c = calculate(0b01, {1,2,3});
	assert(*c == 5);
}
void test_calc_5() {
	auto c = calculate(0b11, {1,2,3});
	assert(*c == 6);
}
void test_calc_6() {
	auto c = calculate(0b10101, {1,2,3,4,5,1});
	assert(*c == 28);
}
void test_calc_7() {
	auto c = calculate(0b1010, {1,2,3,4,5});
	assert(*c == 27);
}

int main() {
	// std::cout << byte_to_binary(7) << std::endl;

	test_calc_1();
	test_calc_2();
	test_calc_3();
	test_calc_4();
	test_calc_6();
	test_calc_7();
	f({2456,5488,3762,6079,6295,748,230}, 51921635);

	return 0;
}
