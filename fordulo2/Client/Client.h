#ifndef _HOME_ESZABPT_NNG2014_FORDULO2_CLIENT_CLIENT_H
#define _HOME_ESZABPT_NNG2014_FORDULO2_CLIENT_CLIENT_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <algorithm>
#include <iostream>

struct ServerResponse {
		std::size_t tickId, gameId;
		std::size_t myId;
		std::size_t cash;
		std::vector<std::size_t> myCards;
		std::size_t numberOfPlayers;
		std::map<std::size_t, std::pair<std::size_t, std::string>> playerStatistics;
		std::size_t pot;
		std::size_t blind;
		std::vector<std::size_t> commonCards;
		std::pair<std::size_t, std::string> lastAction;
		bool isNew = false;
		bool isOver = false;
		std::size_t winner = 0;

		void printPlayers() const {

			std::vector<std::pair<std::size_t, std::string>> players;

			for(const auto& p: playerStatistics) {
				players.push_back(std::make_pair(p.second.first, p.second.second));
			}

			std::sort(players.begin(), players.end(),
			[](auto l, auto r){
				return l.first > r.first;
			});
			for(auto& p: players) {
				std::cerr<<"$"<<p.first<<"\t"<<p.second;
			}
			std::cerr<<std::endl;
			return;
		}
};

struct ResponseHistory {
	std::vector<ServerResponse> responses;
	std::size_t myId;
	std::size_t wins = 0;
	std::size_t lossesWithBets = 0;

	void addServerResponse(ServerResponse r) {
		if(r.isNew) {
			responses.clear();
		}
		responses.push_back(std::move(r));
		myId = r.myId;
		if(r.isOver) {
			if(r.winner == myId) {
				++wins;
			} else {

				auto betIterator = std::find_if(responses.begin(), responses.end(), [&](auto r) {
					return r.lastAction.second == "bet" && r.lastAction.first == myId;
				});

				if (betIterator != responses.end()) {
					++lossesWithBets;
				}

			}
		}
		std::cerr<<"Current round is "<<responses.size()<<" turns old"<<std::endl;
		std::cerr<<"To date, we have been to aggressive: "<<lossesWithBets<< " times, and have won: "<<wins<<" times"<<std::endl;
	}

	std::size_t numberOfEnemyBets() const {
		return std::count_if(responses.begin(), responses.end(), [&](auto a) {
			return a.lastAction.second == "bet" && a.lastAction.first != myId;
		});
	}

	std::size_t numberOfBets() const {
		return std::count_if(responses.begin(), responses.end(), [](auto a) {
			return a.lastAction.second == "bet";
		});
	}


};


class CLIENT
{
public:
	CLIENT();
	virtual ~CLIENT();
	// Initializes the connection to the given server and port
	bool Init( std::string aIPAddress );

	// Runs the client
	void Run();

	std::string DebugResponse(std::vector<std::string> &text) { return HandleServerResponse(text); }

protected:
	void SendMessage( std::string aMessage );
	virtual std::string HandleServerResponse(std::vector<std::string> &ServerResponse) = 0;
	virtual void ConnectionClosed();
	virtual std::string GetPassword() = 0;
	virtual bool NeedDebugLog() = 0;
	std::ofstream mDebugLog;
#ifdef WIN32
	SOCKET mConnectionSocket;
#else
	int mConnectionSocket;
#endif

};

CLIENT *CreateClient();

#endif /* _HOME_ESZABPT_NNG2014_FORDULO2_CLIENT_CLIENT_H */
