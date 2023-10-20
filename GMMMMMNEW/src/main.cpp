#pragma once
#include "Win32_Platform.hpp"
#ifndef GLHDR
#define GLHDR
#include <gl/GL.h>
#endif
#include "platform.h"
#include "input.h"
#include "game.h"
#include <assert.h>

#ifndef KEYLOOKUPTABLE
#define KEYLOOKUPTABLE
static KeyCodeID KeyCodeLookupTable[KEY_COUNT];
#endif


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory>

#include "OpenGLRenderer.hpp"

constexpr auto dllPath = const_cast<char*>("C:\\Users\\Admin\\source\\repos\\GameBuildTest\\GMMMMMNEW\\src\\gameDLLprj.dll");

// DLL FUNCTIONS
typedef decltype(updateGame) updateGameType;
static updateGameType* updateGamePtr;

void reloadGameDLL(BumpAllocator* transientStorage);





int main() {
	platform_fill_keycode_lookup_table();
	char title[] = "GAME";
	BumpAllocator transientStorage = makeBumpAllocator(MB(50));
	BumpAllocator persistentStorage = makeBumpAllocator(MB(50));

	input = (Input*)bumpAlloc(&persistentStorage, sizeof(Input));
	assert(input && "failed to allocate input");

	renderData = (RenderData*)bumpAlloc(&persistentStorage, sizeof(RenderData));
	assert(renderData && "failed to allocate renderData");

	gameState = (GameState*)bumpAlloc(&persistentStorage, sizeof(GameState));
	assert(gameState && "failed to allocate GameState");
	createWindow(1280, 720, &title[0]);
	gl_init(&transientStorage);
	
	updateGamePtr = (updateGameType*)loadDynamicFunction(loadDynamicLibrary(dllPath), (char*)"updateGame");

	


	while (running)
	{
		reloadGameDLL(&transientStorage);
		updateWindow();
		updateGame(gameState,renderData,input);
		gl_render();
		platform_swap_buffers();		

		transientStorage.used = 0;
	};
}

void reloadGameDLL(BumpAllocator* transientStorage)
{
	static void* gameDLL;
	static long long lastEditTimestampGameDLL;

	long long currentTimeStampGameDLL = getTimeStamp("gameDLLprj.dll");
	if (currentTimeStampGameDLL > lastEditTimestampGameDLL)
	{
		if (gameDLL)
		{
			bool freeResult = freeDLL(gameDLL);
			assert(freeResult && "failed to free gameDLL");
			gameDLL = nullptr;
			
		}

		while (!copyFile((char*)"gameDLLprj.dll", (char*)"gameLoad.dll", transientStorage))
		{
			Sleep(10);
		}
		std::cout << "copied gameDLLprj.dll into gameLoad.dll" << "\n";

		gameDLL = loadDynamicLibrary((char*)"gameLoad.dll");

		updateGamePtr = (updateGameType*)loadDynamicFunction(gameDLL, (char*)"updateGame");
		assert(updateGamePtr, "failed to load updateGameFunction");
		lastEditTimestampGameDLL = currentTimeStampGameDLL;
	}


}
void updateGame(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn)
{
	updateGamePtr(gameStateIn,renderDataIn, inputIn);
}