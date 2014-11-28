#pragma once
#include "Client.h"
#include <cassert>
#include <iostream>

const char* preFlop(const ServerResponse& sr) {
	const auto& myCards = sr.myCards;
	if (myCards.size() < 2) return "check";

	for (auto c : myCards) {
		std::cerr << c << " ";
	}
	std::cerr << std::endl;

	auto a = myCards.at(0);
	auto b = myCards.at(1);

	const auto c1 = 5;
	const auto c2 = 7;

	if (a == b) { // par
		if (a > c1) return "bet";
		return "call";
	} else {
		if (a > c2 || b > c2) return "call";
		return "check";
	}
}

