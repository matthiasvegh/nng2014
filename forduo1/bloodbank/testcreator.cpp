#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <boost/lexical_cast.hpp>

int main(int, char** argv) {

	auto n = boost::lexical_cast<std::size_t>(argv[1]);
	std::vector<std::size_t> output(n);

	for(std::size_t i=0; i<n; i++) {
		output[i] = 0;
	}

	std::size_t unhealthy = std::round(double(n)/10.0);

	for(std::size_t i=0; i<unhealthy; ++i) {
		output[i] = 1;
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(output.begin(), output.end(), g);

	std::cout<<n<<std::endl;
	std::cout<<output[0];
	for(std::size_t i=1; i<output.size(); ++i) {
		std::cout<<" "<<output[i];
	}
	std::cout<<std::endl;

}
