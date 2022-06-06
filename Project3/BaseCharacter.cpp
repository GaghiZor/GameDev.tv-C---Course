#include "BaseCharacter.h"
#include "raylib.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{
    //
}

void BaseCharacter::tick(float deltaTime)
{
    worldPosLastFrame = worldPos;

    // Update animation frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
        frame += 1;
        runningTime = 0.0f;
        if (frame > maxFrames)
            frame = 0;
    }

    if (Vector2Length(velocity) != 0.0)
    {
        // set worldPos = worldPos + velocity
        worldPos = Vector2Add(worldPos, Vector2Scale(Vector2Normalize(velocity), speed));
        rightLeft = velocity.x < 0.0f ? -1.0f : 1.0f;
        texture = run;
    }
    else
    {
        texture = idle;
    }
    velocity = {};

    // Draw character
    Rectangle source = {frame * width, 0.0f, rightLeft * width, height};
    Rectangle dest = {getScreenPos().x, getScreenPos().y, characterScale * width, characterScale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.0f, WHITE);
}

void BaseCharacter::undoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{
        getScreenPos().x,
        getScreenPos().y,
        width * characterScale,
        height * characterScale,
    };
}