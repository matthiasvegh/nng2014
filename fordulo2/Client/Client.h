#ifndef _HOME_ESZABPT_NNG2014_FORDULO2_CLIENT_CLIENT_H
#define _HOME_ESZABPT_NNG2014_FORDULO2_CLIENT_CLIENT_H

#include <string>
#include <vector>
#include <fstream>
#include <map>

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
		std::string lastAction;
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
