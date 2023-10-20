#pragma once
#include "fileUtils.h"
//CONSTS________________________________


//STRUCTS_______________________________
enum SpriteID
{
	SPRITE_FROG,
	SPRITE_WHITE,
	SPRITE_COUNT
};


struct Sprite
{
	iVec2 atlasOffset;
	iVec2 spriteSize;
};


//FUNCTIONS_____________________________


Sprite getSprite(SpriteID spriteID)
{
	Sprite sprite = {};


	switch (spriteID)
	{
	case SPRITE_WHITE:
	{
		sprite.atlasOffset = { 32,0 };
		sprite.spriteSize = { 1,1 };
		break;
	}
	case SPRITE_FROG:
	{
		sprite.atlasOffset = { 0,0 };
		sprite.spriteSize = { 32,32 };
		break;
	}
	}

	return sprite;
}