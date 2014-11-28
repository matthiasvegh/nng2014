#ifndef FLOP_H
#define FLOP_H

#include "Client.h"
#include <boost/range/algorithm.hpp>
#include <algorithm>

inline
std::string getFlopResponse(const ServerResponse& data)
{
	std::cerr << "hand: ";
	for (const auto& i: data.myCards) {
		std::cerr << i << " ";
	}
	std::cerr << "\ndesk: ";
	for (const auto& i: data.commonCards) {
		std::cerr << i << " ";
	}
	std::vector<std::size_t> cards = data.myCards;
	boost::copy(data.commonCards, std::back_inserter(cards));
	std::cerr << "\ncards: ";
	for (const auto& i: cards) {
		std::cerr << i << " ";
	}

	std::map<std::size_t, std::size_t> numCards; // mibol -> mennyi
	for (auto card: cards) {
		++numCards[card];
	}

	std::map<std::size_t, std::size_t> cardTypes; // mennyi -> mibol

	for (const auto& value: numCards) {
		cardTypes[value.second] += value.first;
	}

	int value = 0;
	int el = 1;
	auto it = cardTypes.rbegin();
	auto largest = *it;
	switch (largest.first) {
		case 5: el = 5; value = 10000 * largest.second; break;
		case 4: el = 4; value = 2000 * largest.second; break;
		case 3: {
			--it;
			if (it != cardTypes.rend() && it->first == 2) {
				value = 1000 * (largest.second + it->second);
				el = 5;
			} else {
				value = 200 * largest.second;
				el = 3;
			}
			break;
		}
		case 2: {
			--it;
			if (it != cardTypes.rend() && it->first == 2) {
				el = 4;
				value = 10 * (largest.second + it->second);
			} else {
				el = 2;
				value = largest.second;
			}
			break;
		}
	};

	std::cerr << "\nvalue = " << value;

	std::map<std::size_t, std::size_t> numCardsDesk; // mibol -> mennyi
	for (auto card: data.commonCards) {
		++numCardsDesk[card];
	}

	std::size_t fl = 1;
	for (const auto& value: numCardsDesk) {
		if (value.second > 1) {
			fl = std::max(fl, value.second);
		}
	}

	value *= el;
	value /= fl;
	std::cerr << "\nadjusted value = " << value;

	std::cerr << std::endl;

	if (value < 1000) {
		return "check";
	}
	if (value < 5000) {
		return "call";
	}
	return "bet";
}



#endif /* FLOP_H */