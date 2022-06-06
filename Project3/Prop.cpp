#include "Prop.h"

Prop::Prop(Vector2 pos, Texture2D tex) : worldPos(pos), texture(tex)
{
}

void Prop::render(Vector2 charPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, charPos);
    DrawTextureEx(texture, screenPos, 0.0f, scale, WHITE);
}

Rectangle Prop::getCollisionRec(Vector2 charPos)
{
    Vector2 screenPos = Vector2Subtract(worldPos, charPos);
    return Rectangle{
        screenPos.x,
        screenPos.y,
        texture.width * scale,
        texture.height * scale,
    };
}