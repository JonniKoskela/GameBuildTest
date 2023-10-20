#pragma once

#include "renderInterface.h"
#include "input.h"
#include "fileUtils.h"

constexpr int UPDATESPERSECOND = 60;
constexpr double UPDATE_DELAY = 1.0 / UPDATESPERSECOND;
constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;
constexpr int TILESIZE = 8;
constexpr iVec2 WORLD_GRID = { WORLD_WIDTH / TILESIZE, WORLD_HEIGHT / TILESIZE };


enum GameInputType {
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	JUMP,

	MOUSE_LEFT,
	MOUSE_RIGHT,

	PAUSE,
	SHIFT,

	GAME_INPUT_COUNT
};

struct Tile
{
	int neighbourMask;
	bool isVisible;
};


struct KeyMapping {
	Array<KeyCodeID, 3> keys;
};

struct GameState
{
	float updateTimer;
	KeyMapping keyMappings[GAME_INPUT_COUNT];
	bool initialized = false;
	iVec2 playerPos = {0,0};
	Array<iVec2, 21> tileCoords;
	Tile worldGrid[WORLD_GRID.y][WORLD_GRID.y];
};

static GameState* gameState = new GameState; // <------------------------------------------------------------------------------------------------


extern "C"
{
	_declspec(dllexport) void updateGame(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float dt);
}


