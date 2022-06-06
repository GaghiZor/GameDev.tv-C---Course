#include "raylib.h"
#include "Enemy.h"
#include "Character.h"
#include "raymath.h"

Enemy::Enemy(Vector2 pos, Texture2D idleTexture, Texture2D runTexture)
{
    worldPos = pos;
    texture = idleTexture;
    idle = idleTexture;
    run = runTexture;

    width = texture.width / maxFrames;
    height = texture.height;
    speed = 4.0f;
}

void Enemy::tick(float deltaTime)
{
    if (!getAlive())
        return;
    // Get toTarget
    velocity = Vector2Subtract(target->getScreenPos(), getScreenPos());
    if(Vector2Length(velocity) < radius) 
    {
        velocity = {};
    }

    BaseCharacter::tick(deltaTime);

    if (CheckCollisionRecs(target->getCollisionRec(), getCollisionRec()))
    {
        target->takeDamage(damagePerSec * deltaTime);
    }
}

Vector2 Enemy::getScreenPos()
{
    return Vector2Subtract(worldPos, target->getWorldPos());
}

void Enemy::setTarget(Character *character)
{
    target = character;
}