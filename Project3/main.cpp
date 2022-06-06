#include <string>

#include "raylib.h"
#include "raymath.h"

#include "Character.h"
#include "Prop.h"
#include "Enemy.h"

int main()
{
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 800;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Top down 2D Game");

    Texture2D map = LoadTexture("resources/map.png");
    Vector2 mapPos = {0.0, 0.0};
    const float mapScale = 4.0f;

    Character character = {WINDOW_WIDTH, WINDOW_HEIGHT};

    Prop props[2] = {
        Prop{Vector2{1100.0f, 800.0f}, LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.0f, 800.0f}, LoadTexture("nature_tileset/Log.png")},
    };

    Enemy goblin = {
        Vector2{400, 1200},
        LoadTexture("characters/goblin_idle_spritesheet.png"),
        LoadTexture("characters/goblin_run_spritesheet.png"),
    };

    Enemy slime = {
        Vector2{700, 750},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png"),
    };

    Enemy *enemies[] = {
        &goblin,
        &slime,
    };

    for (auto enemy : enemies)
    {
        enemy->setTarget(&character);
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        float deltaTime = GetFrameTime();

        mapPos = Vector2Scale(character.getWorldPos(), -1.0f);

        // Draw map
        DrawTextureEx(map, mapPos, 0.0f, mapScale, WHITE);

        // Draw the props
        for (auto prop : props)
        {
            prop.render(character.getWorldPos());
        }

        if (!character.getAlive())
        {
            // Character is not alove
            DrawText("Game Over!", 55, 45, 40, RED);
            EndDrawing();
            continue;
        }
        else
        {
            // Character is alive
            std::string characterHealth = "Health: ";
            characterHealth.append(std::to_string(character.getHealth()), 0, 5);
            DrawText(characterHealth.c_str(), 55, 45, 40, RED);
        }

        character.tick(deltaTime);

        // Check map bounds
        if (character.getWorldPos().x < 0.0f ||
            character.getWorldPos().y < 0.0f ||
            character.getWorldPos().x + WINDOW_WIDTH > map.width * mapScale ||
            character.getWorldPos().y + WINDOW_HEIGHT > map.height * mapScale)
        {
            character.undoMovement();
        }

        // Check prop collisions
        for (auto prop : props)
        {
            if (CheckCollisionRecs(character.getCollisionRec(), prop.getCollisionRec(character.getWorldPos())))
                character.undoMovement();
        }

        for (auto enemy : enemies)
        {
            enemy->tick(deltaTime);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for (auto enemy : enemies)
            {
                if (CheckCollisionRecs(enemy->getCollisionRec(), character.getWeaponCollisionRec()))
                {
                    enemy->setAlive(false);
                }
            }
        }

        EndDrawing();
    }
    UnloadTexture(map);
    CloseWindow();

    return 0;
}