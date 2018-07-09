#include "network.h"
#include "steam/steam_api.h"
#include <fstream>

void Network::createLobby() {
	SteamAPICall_t createLobbyCall = SteamMatchmaking()->CreateLobby(k_ELobbyTypePublic, 2);
	callCreateResult.Set(createLobbyCall, this, &Network::onCreateLobby);
}

void Network::onCreateLobby(LobbyCreated_t* result, bool failure) {
	isLobbyMade = true;
	std::ofstream writeFile;
	writeFile.open("log.txt");
	if (!result->m_eResult == k_EResultOK || failure) {
		writeFile << "lobby creation failed";
		writeFile.close();
		writeFile.clear();
		return;
	}
	lobbyID = result->m_ulSteamIDLobby;
	writeFile << "lobby creation succeeded, id = " << lobbyID;
	writeFile.close();
	writeFile.clear();
}

void Network::joinLobby(CSteamID steamIDLobby) {
	SteamAPICall_t joinLobbyCall = SteamMatchmaking()->JoinLobby(steamIDLobby);
	callJoinResult.Set(joinLobbyCall, this, &Network::onJoinLobby);
}

void Network::onJoinLobby(LobbyEnter_t* result, bool failure) {
	isLobbyMade = true;
	std::ofstream writeFile;
	writeFile.open("log.txt");
	if (!result->m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess || failure) {
		writeFile << "lobby join failed";
		writeFile.close();
		writeFile.clear();
		return;
	}
	lobbyID = result->m_ulSteamIDLobby;
	writeFile << "lobby join succeeded, id = " << lobbyID;
	writeFile.close();
	writeFile.clear();
}