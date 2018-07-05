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

#define SDL

#include "steam/steam_api.h"
#include <SDL.h>
#include <cstddef>
#include <Windows.h>
#ifdef OPENGL
#include "overhead.h"
#include "image.h"
#endif
#ifdef SDL
#include <SDL_image.h>
#endif
#include <fstream>

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

//-----------------------------------------------------------------------------
// Purpose: Extracts some feature from the command line
//-----------------------------------------------------------------------------
void parseCommandLine( const char *pchCmdLine, const char **ppchServerAddress, const char **ppchLobbyID ) {
	// Look for the +connect ipaddress:port parameter in the command line,
	// Steam will pass this when a user has used the Steam Server browser to find
	// a server for our game and is trying to join it.
	const char *pchConnectParam = "+connect";
	const char *pchConnect = strstr( pchCmdLine, pchConnectParam );
	*ppchServerAddress = NULL;
	if ( pchConnect && strlen( pchCmdLine ) > (pchConnect - pchCmdLine) + strlen( pchConnectParam ) + 1 ) {
		// Address should be right after the +connect, +1 on the end to skip the space
		*ppchServerAddress = pchCmdLine + ( pchConnect - pchCmdLine ) + strlen( pchConnectParam ) + 1;
	}
	// look for +connect_lobby lobbyid paramter on the command line
	// Steam will pass this in if a user taken up an invite to a lobby
	const char *pchConnectLobbyParam = "+connect_lobby";
	const char *pchConnectLobby = strstr( pchCmdLine, pchConnectParam );
	*ppchLobbyID = NULL;
	if ( pchConnectLobby && strlen( pchCmdLine ) > (pchConnectLobby - pchCmdLine) + strlen( pchConnectLobbyParam ) + 1 ) {
		// Address should be right after the +connect, +1 on the end to skip the space
		*ppchLobbyID = pchCmdLine + ( pchConnectLobby - pchCmdLine ) + strlen( pchConnectLobbyParam ) + 1;
	}
}

int main(int argc, char* args[]) {
	std::ifstream readFile;
	readFile.open("host.txt");
	char checkHost;
	readFile >> checkHost;
	readFile.close();
	readFile.clear();
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
		char input;
		for (int b = 0, input = '-'; input != '\0'; ++b) {
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
	parseCommandLine(pchCmdLine, &pchServerAddress, &pchLobbyID);
	std::ofstream writeFile;
	writeFile.open("log2.txt");
	if (pchLobbyID != NULL)
		writeFile << pchLobbyID;
	else
		writeFile << "nothing in pchLobbyID";
	if (pchCmdLine[0] != '\0')
		writeFile << pchCmdLine;
	else
		writeFile << "nothing on command line";
	writeFile.close();
	writeFile.clear();
	if (checkHost == 'y') {
		network.setIsLobbyMade(false);
		network.createLobby();
	}
	else {
		network.setIsLobbyMade(false);
		CSteamID lobbyID((uint64)_atoi64(pchLobbyID));
		network.joinLobby(lobbyID);
	}
	while (network.isLobbyMade == false) {
		SteamAPI_RunCallbacks();
	}
	int screenWidth = 800;
	int screenHeight = 600;
#ifdef OPENGL
	size_t screenSize = screenHeight * screenWidth * 4;
	uint8_t* screen = new uint8_t[screenSize];
	Overhead overhead(screenWidth, screenHeight, "opengl test", screen);
	int imageNum = 1;
	Image* image = new Image[imageNum];
	image[0].setupImage("test.img");
#endif
#ifdef SDL
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("steam sdl test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	IMG_Init(IMG_INIT_PNG);
	SDL_Surface* surface = IMG_Load("test.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
#endif
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT)
				quit = true;
		}
#ifdef OPENGL
		image[0].writePixels(screen, 0, 0, screenWidth, screenHeight);
		overhead.render(screen);
#endif
#ifdef SDL
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
#endif
	}
	SteamMatchmaking()->LeaveLobby(network.lobbyID);
	SteamAPI_Shutdown();
#ifdef OPENGL
	delete[] screen;
	delete[] image;
#endif
#ifdef SDL
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
#endif
	return 0;
}