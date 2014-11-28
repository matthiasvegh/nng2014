#include "stdafx.h"
#include "Client.h"
#include "preFlop.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

// sample

const char *CommandList[]={ "check", "call", "bet" };

class MYCLIENT : public CLIENT
{
public:
	MYCLIENT();
protected:
	virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse);
	virtual std::string GetPassword() { return std::string("BpN8J8"); } // Nodus
	virtual bool NeedDebugLog() { return true; }
};

MYCLIENT::MYCLIENT()
{
}

void trimAll(std::vector<std::string>& s) {
	for(auto& s_: s) {
		boost::trim(s_);
	}
}

ServerResponse parseServerResponse(const std::vector<std::string>& serverResponse) {
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
	response.myCards = {
		boost::lexical_cast<std::size_t>(line[1]),
		boost::lexical_cast<std::size_t>(line[2])
	};
	line.clear();
	// handle map here

	boost::split(line, serverResponse[4], boost::is_any_of(" "));
	trimAll(line);
	response.numberOfPlayers = boost::lexical_cast<std::size_t>(line[1]);
	std::size_t endOfPlayers = 5+response.numberOfPlayers;
	line.clear();

	boost::split(line, serverResponse[endOfPlayers], boost::is_any_of(" "));
	trimAll(line);
	response.pot = boost::lexical_cast<std::size_t>(line[1]);
	line.clear();

	boost::split(line, serverResponse[endOfPlayers+1], boost::is_any_of(" "));
	trimAll(line);
	response.blind = boost::lexical_cast<std::size_t>(line[1]);
	line.clear();

	// get extra optional fields
	return response;
}

std::string MYCLIENT::HandleServerResponse(std::vector<std::string> &serverResponse)
{
	ServerResponse response = parseServerResponse(serverResponse);
	return preFlop(response);
}


CLIENT *CreateClient()
{
	return new MYCLIENT();
}
