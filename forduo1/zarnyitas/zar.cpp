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

struct Status {
	Num result = 0;
	Num subResult = 1;
	int i = 0;
};

struct F {
	std::vector<Num> v;
	Num expectedResult;
	const std::size_t numberOfBits = v.size() - 1;
	const Rep size = std::pow(2, numberOfBits);
	const Rep hundredth = size / 100;
	int percent;
	Rep solved = 0;

	F(const std::vector<Num>& v, Num expectedResult):
			v(v), expectedResult(expectedResult)
	{}

	void advance(Rep n) {
		solved += n;

		int newPercent = solved / hundredth;

		if (newPercent != percent) {
			percent = newPercent;
			std::cerr << percent << "%\r";
		}

	}

	bool solve(Num result, Num subResult, int i) {
		subResult *= v[i];

		if (result + subResult > expectedResult) {
			advance(std::pow(2, i));
			return false;
		}

		if (i == 0) {
			advance(1);
			result += subResult;

			return result == expectedResult;
		}

		if (solve(result, subResult, i-1)) {
			std::cout << "*";
			return true;
		}

		if (solve(result + subResult, 1, i-1)) {
			std::cout << "+";
			return true;
		}

		return false;
	}

	void operator()() {
		solve(0, 1, v.size() - 1);
	}
};

Num numAtoi(const char* const str) {
	Num res = 0; // Initialize result
	for (int i = 0; str[i] != '\0'; ++i) res = res * 10 + str[i] - '0';
	return res;
}

int main(int argc, char* argv[]) {
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
