#include "raylib.h"
#include "raymath.h"

class Prop
{
public:
    Prop(Vector2 pos, Texture2D tex);
    void render(Vector2 charPos);
    Rectangle getCollisionRec(Vector2 charPos);
private:
    Vector2 worldPos = {};
    Texture2D texture = {};
    float scale = 4.0f;
};