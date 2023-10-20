#pragma once
#include "fileUtils.h"
#include "assets.h"
#include "input.h"

struct OrtographicCamera2D {
	Vec2 position;
	Vec2 dimensions;
	float zoom = 1.0f;
};

struct Transform {
	Vec2 size;
	Vec2 pos;
	iVec2 atlasOffset;
	iVec2 spriteSize;
};

struct RenderData
{
	OrtographicCamera2D gameCamera;
	OrtographicCamera2D uiCamera;
	Array<Transform, 1000> transforms;
};

static RenderData* renderData = new RenderData;

iVec2 screenToWorld(iVec2 screenPos)
{
	OrtographicCamera2D camera = renderData->gameCamera;

	int xPos = (float)screenPos.x / 
		(float)input->screenSize.x * camera.dimensions.x;

	xPos += -camera.dimensions.x / 2.0f + camera.position.x;

	int yPos = (float)screenPos.y / 
		(float)input->screenSize.y * camera.dimensions.y;

	yPos += camera.dimensions.y / 2.0f + camera.position.y;

	return { xPos, yPos };
}



void drawQuad(Vec2 pos, Vec2 size)
{
	Transform transform = {};
	transform.pos = pos - size / 2.0f;
	transform.size = size;
	transform.atlasOffset = { 32,0 };
	transform.spriteSize = { 1,1 };

	renderData->transforms.add(transform);
}
void drawQuad(Transform transform)
{
	renderData->transforms.add(transform);
}




void drawSprite(SpriteID spriteID, Vec2 pos)
{
	Sprite sprite = getSprite(spriteID);

	Transform transform = {};
	transform.pos = pos - vec_2(sprite.spriteSize) / 2.0f;
	transform.size = vec_2(sprite.spriteSize);
	transform.atlasOffset = sprite.atlasOffset;
	transform.spriteSize = sprite.spriteSize;

	renderData->transforms.add(transform);
}

void drawSprite(SpriteID spriteID, iVec2 pos)
{
	drawSprite(spriteID,vec_2(pos));
}