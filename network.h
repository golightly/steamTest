#pragma once

#include "steam/steam_api.h"

class Network {
public:
	void onCreateLobby(LobbyCreated_t* result, bool failure);
	void createLobby();
	CCallResult<Network, LobbyCreated_t> callCreateResult;
	uint64 lobbyID;
	bool isLobbyMade;
	inline void setIsLobbyMade(bool isLobbyMade) { this->isLobbyMade = isLobbyMade; }
	void onJoinLobby(LobbyEnter_t* result, bool failure);
	void joinLobby(CSteamID steamIDLobby);
	CCallResult<Network, LobbyEnter_t> callJoinResult;
};