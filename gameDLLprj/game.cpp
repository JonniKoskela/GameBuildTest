// dllmain.cpp : Defines the entry point for the DLL application.
#pragma once
#include "pch.h"
#include "../GMMMMMNEW/src/assets.h"
#include "../GMMMMMNEW/src/game.h"
#include "../GMMMMMNEW/src/fileUtils.h"
// CONSTANTS*************************************************************************************

// STRUCTS***************************************************************************************

// FUNCTIONS*************************************************************************************



bool just_pressed(GameInputType type)
{
	KeyMapping mapping = gameState->keyMappings[type];
	for (int idx = 0; idx < mapping.keys.count; idx++)
	{
		if (input->keys[mapping.keys[idx]].justPressed)
		{
			return true;
		}
	}

	return false;
}
bool isDown(GameInputType type)
{
	KeyMapping mapping = gameState->keyMappings[type];
	for (int idx = 0; idx < mapping.keys.count; idx++)
	{
		if (input->keys[mapping.keys[idx]].isDown)
		{
			return true;
		}
	}

	return false;
}


Tile* get_tile(int x, int y)
{
	Tile* tile = nullptr;

		if (x >= 0 && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y)
		{
			tile = &gameState->worldGrid[x][y];
		}

		return tile;
}
Tile* get_tile(iVec2 worldPos)
{
	int x = worldPos.x / TILESIZE;
	int y = worldPos.y / TILESIZE;

	return get_tile(x, y);
}





inline void drawTileSet(GameState* gameState)
{
	for (int y = 0; y < WORLD_GRID.y; y++)
	{
		for (int x = 0; x < WORLD_GRID.x; x++)
		{
			Tile* tile = get_tile(x, y);

			if (!tile->isVisible)
			{
				continue;
			}

			Transform transform = {};
			transform.pos = { x * (float)TILESIZE, y * (float)TILESIZE };
			transform.size = { 8,8 };
			transform.spriteSize = { 8,8 };
			transform.atlasOffset = gameState->tileCoords[tile->neighbourMask];
			drawQuad(transform);
		}
	}
}



inline void initializeGame(RenderData* renderData, GameState* gameState)
{
	renderData->gameCamera.dimensions = { WORLD_WIDTH,WORLD_HEIGHT };
	gameState->initialized = true;
	{
		gameState->keyMappings[MOVE_UP].keys.clear();
		gameState->keyMappings[MOVE_UP].keys.add(KEY_W);
		gameState->keyMappings[MOVE_UP].keys.add(KEY_UP);
		gameState->keyMappings[MOVE_LEFT].keys.add(KEY_A);
		gameState->keyMappings[MOVE_LEFT].keys.add(KEY_LEFT);
		gameState->keyMappings[MOVE_DOWN].keys.add(KEY_S);
		gameState->keyMappings[MOVE_DOWN].keys.add(KEY_DOWN);
		gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_D);
		gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_RIGHT);
		gameState->keyMappings[MOUSE_LEFT].keys.add(KEY_MOUSE_LEFT);
		gameState->keyMappings[MOUSE_RIGHT].keys.add(KEY_MOUSE_RIGHT);
		gameState->keyMappings[JUMP].keys.add(KEY_SPACE);
		gameState->keyMappings[PAUSE].keys.add(KEY_ESCAPE);
		gameState->keyMappings[SHIFT].keys.add(KEY_SHIFT);
	}
	renderData->gameCamera.position.x += 160;
	renderData->gameCamera.position.y += -90;
	{
		iVec2 tilesPosition = { 96,0 };
		for (int y = 0; y < 5; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				gameState->tileCoords.add({ tilesPosition.x + x * 8, tilesPosition.y + y * 8 });
			}
		}
		gameState->tileCoords.add({ tilesPosition.x,tilesPosition.y + 5 * 8 });
	}
}



void simulate() 
{
	if (isDown(MOVE_LEFT))
	{
		gameState->playerPos.x -= 1;
	}
	if (isDown(MOVE_RIGHT))
	{
		gameState->playerPos.x += 1;
	}
	if (isDown(MOVE_UP))
	{
		gameState->playerPos.y -= 1;
	}
	if (isDown(MOVE_DOWN))
	{
		gameState->playerPos.y += 1;
	}
	bool updateTiles = false;
	if (isDown(MOUSE_LEFT))
	{
		iVec2 mousePosWorld = input->mousePosWorld;
		Tile* tile = get_tile(mousePosWorld);
		if (tile)
		{
			tile->isVisible = true;
			updateTiles = true;
		}
	}
	if (isDown(MOUSE_RIGHT))
	{
		iVec2 mousePosWorld = input->mousePosWorld;
		Tile* tile = get_tile(mousePosWorld);
		if (tile)
		{
			tile->isVisible = false;
			updateTiles = true;
		}
	}
	if (updateTiles)
	{
	int neighbourOffsets[24] = { 0,-1,  -1, 0,     1, 0,       0, 1,
								-1,-1,   1,-1,    -1, 1,       1, 1,
								 0,-2,  -2, 0,     2, 0,       0, 2 };
		for (int y = 0; y < WORLD_GRID.y; y++)
		{
			for (int x = 0; x < WORLD_GRID.x; x++)
			{
				Tile* tile = get_tile(x, y);

				if (!tile->isVisible)
				{
					continue;
				}

				tile->neighbourMask = 0;
				int neighbourCount = 0;
				int extendedNeighbourCount = 0;
				int emptyNeighbourSlot = 0;

				// Look at the sorrounding 12 Neighbours
				for (int n = 0; n < 12; n++)
				{
					Tile* neighbour = get_tile(x + neighbourOffsets[n * 2],
						y + neighbourOffsets[n * 2 + 1]);

					// No neighbour means the edge of the world
					if (!neighbour || neighbour->isVisible)
					{
						tile->neighbourMask |= BIT(n);
						if (n < 8)
						{
							neighbourCount++;
						}
						else
						{
							extendedNeighbourCount++;
						}
					}
					else if (n < 8)
					{
						emptyNeighbourSlot = n;
					}
				}

				if (neighbourCount == 7 && emptyNeighbourSlot >= 4)
				{
					tile->neighbourMask = 16 + (emptyNeighbourSlot - 4);
				}
				else if (neighbourCount == 8 && extendedNeighbourCount == 4)
				{
					tile->neighbourMask = 20;
				}
				else
				{
					tile->neighbourMask = tile->neighbourMask & 0b1111;
				}
			}
		}
	}
}



// ACTUAL UPDATE GAME LOOP



_declspec(dllexport) void updateGame(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float dt)
{
	if (renderDataIn != renderData)
	{
		gameState = gameStateIn;
		renderData = renderDataIn;
		input = inputIn;
	}
	if (!gameState->initialized)
	{
		initializeGame(renderDataIn,gameStateIn);
	}

	{
		gameState->updateTimer += dt;
		while (gameState->updateTimer >= UPDATE_DELAY)
		{
			gameState->updateTimer -= UPDATE_DELAY;
			simulate();

			input->relMouse = input->mousePos - input->prevMousePos;
			input->prevMousePos = input->mousePos;


			for (int keyCode = 0; keyCode < KEY_COUNT; keyCode++)
			{
				input->keys[keyCode].justReleased = false;
				input->keys[keyCode].justPressed = false;
				input->keys[keyCode].halfTransitionCount = 0;
			}
		}
	}
	float interpolatedDT = (float)(gameState->updateTimer / UPDATE_DELAY);

	drawTileSet(gameStateIn);
	drawSprite(SPRITE_FROG, gameState->playerPos);
}