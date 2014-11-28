#include "stdafx.h"
#include "Client.h"
#include "preFlop.h"
#include "flop.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// sample

const char* CommandList[] = {"check", "call", "bet"};

class MYCLIENT : public CLIENT {
public:
	MYCLIENT();

protected:
	virtual std::string HandleServerResponse(
		std::vector<std::string>& ServerResponse);
	virtual std::string GetPassword() { return std::string("BpN8J8"); } // Nodus
	virtual bool NeedDebugLog() { return true; }
};

MYCLIENT::MYCLIENT() {}

void trimAll(std::vector<std::string>& s) {
	for (auto& s_ : s) {
		boost::trim(s_);
	}
}

ServerResponse parseServerResponse(std::vector<std::string> serverResponse) {
	ServerResponse response;
	std::vector<std::string> line;

	boost::split(line, serverResponse[0], boost::is_any_of(" "));

	boost::trim(line[2]);

	response.tickId = boost::lexical_cast<std::size_t>(line[1]);
	response.gameId = boost::lexical_cast<std::size_t>(line[2]);
	line.clear();

	boost::split(line, serverResponse[1], boost::is_any_of(" \n"));
	trimAll(line);
	response.myId = boost::lexical_cast<std::size_t>(line[1]);
	line.clear();

	boost::split(line, serverResponse[2], boost::is_any_of(" \n"));
	trimAll(line);
	response.cash = boost::lexical_cast<std::size_t>(line[1]);
	line.clear();

	boost::split(line, serverResponse[3], boost::is_any_of(" \n"));
	trimAll(line);
	response.myCards = {boost::lexical_cast<std::size_t>(line[1]),
						boost::lexical_cast<std::size_t>(line[2])};
	line.clear();

	boost::split(line, serverResponse[4], boost::is_any_of(" "));
	trimAll(line);
	response.numberOfPlayers = boost::lexical_cast<std::size_t>(line[1]);
	std::size_t endOfPlayers = 5 + response.numberOfPlayers;
	line.clear();

	for (std::size_t i = 5; i < response.numberOfPlayers + 5; ++i) {
		std::stringstream ss;
		ss << serverResponse[i] << std::endl;
		int id, cash;
		ss >> id >> cash;
		std::string name(ss.str());
		response.playerStatistics[id] = std::make_pair(cash, name);
	}

	boost::split(line, serverResponse[endOfPlayers], boost::is_any_of(" "));
	trimAll(line);
	response.pot = boost::lexical_cast<std::size_t>(line[1]);
	line.clear();

	boost::split(line, serverResponse[endOfPlayers + 1], boost::is_any_of(" "));
	trimAll(line);
	response.blind = boost::lexical_cast<std::size_t>(line[1]);
	line.clear();

	std::size_t numberOfCards = 0;
	// get extra optional fields
	auto it = std::find_if(serverResponse.begin(), serverResponse.end(), [&](auto l) {
		boost::split(line, l, boost::is_any_of(" "));
		if(line[0] == "cards") {
			boost::trim(line[1]);
			numberOfCards = boost::lexical_cast<std::size_t>(line[1]);
			return true;
		}
		return false;
	});

	std::size_t index = std::distance(serverResponse.begin(), it);

	line.clear();

	for(std::size_t i = 1; i< numberOfCards+1; ++i) {
		boost::trim(serverResponse[index+i]);
		response.commonCards.push_back( 
				boost::lexical_cast<std::size_t>(serverResponse[index+i]));
	}

	line.clear();

	auto actionIt = std::find_if(serverResponse.begin(), serverResponse.end(), [&](auto l) {
		std::deque<std::string> line;
		boost::split(line, l, boost::is_any_of(" "));
		if(line[0] == "action") {
			std::size_t player = boost::lexical_cast<std::size_t>(line[1]);
			boost::trim(line[2]);
			std::string action = line[2];

			response.lastAction = std::make_pair(player, action);
			return true;
		}
		return false;
	});

	if(actionIt == serverResponse.end()) {
		response.isNew = true;
	}

	auto winnerIt = std::find_if(serverResponse.begin(), serverResponse.end(), [&](auto l) {
		std::deque<std::string> line;
		boost::split(line, l, boost::is_any_of(" "));
		if(line[0] == "winner") {
			boost::trim(line[1]);
			response.winner = boost::lexical_cast<std::size_t>(line[1]);
			return true;
		}
		return false;
	});

	if(winnerIt == serverResponse.end()) {
		response.isOver = true;
	}

	return response;
}

std::string flopDispatch(const ServerResponse& sr) {
	std::cerr << "cc size: " << sr.commonCards.size() << std::endl;
	if (sr.commonCards.size() == 3) {
		std::cerr << "FLOP" << std::endl;
		return getFlopResponse(sr);
	} else {
		return preFlop(sr);
	}
}

std::string MYCLIENT::HandleServerResponse(
	std::vector<std::string>& serverResponse) {
	ServerResponse response = parseServerResponse(serverResponse);
	static ResponseHistory currentHistory;
	currentHistory.addServerResponse(response);
	std::cerr<<"Current number of bets: "<<currentHistory.numberOfBets()<<std::endl;
	std::cerr<<"Current number of enemy bets: "<<currentHistory.numberOfEnemyBets()<<std::endl;

	response.printPlayers();
	auto resp = flopDispatch(response);
	std::cerr << "Resp : " << resp << std::endl;
	return resp;
}

CLIENT* CreateClient() { return new MYCLIENT(); }
