//====== Copyright � 1996-2008, Valve Corporation, All rights reserved. =======
//
// Purpose: Main file for the SteamworksExample app
//
//=============================================================================

/*to do:
test (july 3, done)
opengl version (july 3, done)
test (july 3, done)
minimal network test (july 4, setup, needs testing)
research sfml voice chat (?) (july 4, done partially, see bookmarks)
write minimal voice chat program (july 5)
write minimal realtime networking test program with voice chat (july 5)
sdk... (elaborate on this)
*/

#include "definitions.h"

#include "steam/steam_api.h"
#include <SDL.h>
#include <cstddef>
#include <Windows.h>
#include <fstream>
#include "network.h"

//-----------------------------------------------------------------------------
// Purpose: Extracts some feature from the command line
//-----------------------------------------------------------------------------
void parseCommandLine( const char* pchCmdLine, const char*& ppchServerAddress, const char*& ppchLobbyID ) {
	// Look for the +connect ipaddress:port parameter in the command line,
	// Steam will pass this when a user has used the Steam Server browser to find
	// a server for our game and is trying to join it.
	const char *pchConnectParam = "+connect";
	const char *pchConnect = strstr( pchCmdLine, pchConnectParam );
	ppchServerAddress = NULL;
	if ( pchConnect && strlen( pchCmdLine ) > (pchConnect - pchCmdLine) + strlen( pchConnectParam ) + 1 ) {
		// Address should be right after the +connect, +1 on the end to skip the space
		ppchServerAddress = pchCmdLine + ( pchConnect - pchCmdLine ) + strlen( pchConnectParam ) + 1;
	}
	// look for +connect_lobby lobbyid paramter on the command line
	// Steam will pass this in if a user taken up an invite to a lobby
	const char *pchConnectLobbyParam = "+connect_lobby";
	const char *pchConnectLobby = strstr( pchCmdLine, pchConnectParam );
	ppchLobbyID = NULL;
	if ( pchConnectLobby && strlen( pchCmdLine ) > (pchConnectLobby - pchCmdLine) + strlen( pchConnectLobbyParam ) + 1 ) {
		// Address should be right after the +connect, +1 on the end to skip the space
		ppchLobbyID = pchCmdLine + ( pchConnectLobby - pchCmdLine ) + strlen( pchConnectLobbyParam ) + 1;
	}
}

int main(int argc, char* args[]) {
	Network network;
	CSteamID localID;
	if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
		return 0;
	if (!SteamAPI_Init())
		return 0;
	if (!SteamUser()->BLoggedOn())
		return 0;
	else
		localID = SteamUser()->GetSteamID();
	char pchCmdLine[1024];
	pchCmdLine[0] = '\0';
	int current = 0;
	for (int a = 1; a < argc; ++a) {
		char input = '-';
		for (int b = 0; input != '\0'; ++b) {
			input = args[a][b];
			pchCmdLine[current] = input;
			++current;
		}
		if (a + 1 != argc) {
			pchCmdLine[current] = ' ';
			++current;
		}
		else
			pchCmdLine[current] = '\0';
	}
	const char *pchServerAddress, *pchLobbyID;
	pchServerAddress = NULL;
	pchLobbyID = NULL;
	parseCommandLine(pchCmdLine, pchServerAddress, pchLobbyID);
	if (pchLobbyID == NULL || pchLobbyID == "") {
		network.setIsLobbyMade(false);
		network.createLobby();
	}
	else {
		char input;
		std::ifstream readFile;
		readFile.open("lobbyID.txt");
		while (readFile >> input) {
			pchLobbyID += input;
		}
		readFile.close();
		readFile.clear();
		network.setIsLobbyMade(false);
		CSteamID lobbyID((uint64)_atoi64(pchLobbyID));
		network.joinLobby(lobbyID);
	}
	while (network.isLobbyMade == false) {
		SteamAPI_RunCallbacks();
	}
	int screenWidth = 800;
	int screenHeight = 600;
	int imageNum = 1;
	Overhead overhead(screenWidth, screenHeight, "steam test");
	if (overhead.renderer == NULL) {
		std::cout << "renderer null in main" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "renderer null in main\n";
		writeFile.close();
		writeFile.clear();
	}
	else if (overhead.renderer != NULL) {
		std::cout << "renderer good in main" << std::endl;
		std::ofstream writeFile;
		writeFile.open("errorLog.txt", std::ios::app);
		writeFile << "renderer good in main\n";
		writeFile.close();
		writeFile.clear();
	}
	const char** imagePath;
	imagePath = new const char*[imageNum];
	imagePath[0] = "test";
	Image* image = new Image[imageNum];
	for (int a = 0; a < imageNum; ++a) {
		IMAGE_SETUP
	}
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				quit = true;
		}
		overhead.render(imageNum, image);
	}
	SteamMatchmaking()->LeaveLobby(network.lobbyID);
	SteamAPI_Shutdown();
	delete[] image;
	delete[] imagePath;
	return 0;
}