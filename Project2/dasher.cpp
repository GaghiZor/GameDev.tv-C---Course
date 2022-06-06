#include "raylib.h"

struct AnimData
{
    Rectangle rectangle;
    Vector2 position;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return (data.position.y >= windowHeight - data.rectangle.height);
}

AnimData updateAnimData(AnimData data, int maxFrame, float deltaTime)
{
    // Update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0.0;

        // Update animation frame
        data.rectangle.x = data.frame * data.rectangle.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // Window dimensions
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    // Initialize window
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Dasher");

    // Acceleration due to gravity (pixels/s)/s
    const int gravity = 2'000;
    // Jump velocity (pixels/s)
    const int JUMP_VELOCITY = -900;

    // Character
    Texture2D character = LoadTexture("textures/scarfy.png");
    // AnimData for character
    AnimData characterData;
    characterData.rectangle.width = character.width / 6;
    characterData.rectangle.height = character.height;
    characterData.rectangle.x = 0;
    characterData.rectangle.y = 0;
    characterData.position.x = WINDOW_WIDTH / 2 - characterData.rectangle.width / 2;
    characterData.position.y = WINDOW_HEIGHT - characterData.rectangle.height;
    characterData.frame = 0.0;
    characterData.updateTime = 1.0 / 12.0;
    characterData.runningTime = 0.0;

    // Obstacle (Nebula)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae = 6;
    AnimData nebulae[sizeOfNebulae] = {};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rectangle.x = 0.0;
        nebulae[i].rectangle.y = 0.0;
        nebulae[i].rectangle.width = nebula.width / 8;
        nebulae[i].rectangle.height = nebula.height / 8;
        nebulae[i].position.y = WINDOW_HEIGHT - nebula.height / 8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].position.x = WINDOW_WIDTH + i * 400;
    }

    float finishLine = nebulae[sizeOfNebulae - 1].position.x;

    // Nebula X velocity (pixels/second)
    int nebulaVelocity = -300;

    int velocity = 0;

    bool isInAir = false;

    Texture2D background = LoadTexture("textures/far-buildings.png");
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float bgX = 0.0;
    float mgX = 0.0;
    float fgX = 0.0;
    float backgroundScale = 3.2;

    bool collision = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // Delta time (time since last frame)
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        // Scroll background
        bgX -= 50 * deltaTime;
        if (bgX <= -background.width * backgroundScale)
        {
            bgX = 0.0;
        }

        // Scroll midground
        mgX -= 100 * deltaTime;
        if (mgX <= -midground.width * backgroundScale)
        {
            mgX = 0.0;
        }

        // Scroll foreground
        fgX -= 200 * deltaTime;
        if (fgX <= -foreground.width * backgroundScale)
        {
            fgX = 0.0;
        }

        // Draw the background
        Vector2 bg1Position = {bgX, 0.0};
        DrawTextureEx(background, bg1Position, 0.0, backgroundScale, WHITE);
        Vector2 bg2Position = {bgX + background.width * backgroundScale, 0.0};
        DrawTextureEx(background, bg2Position, 0.0, backgroundScale, WHITE);

        // Draw the midground
        Vector2 mg1Position = {mgX, 0.0};
        DrawTextureEx(midground, mg1Position, 0.0, backgroundScale, WHITE);
        Vector2 mg2Position = {mgX + background.width * backgroundScale, 0.0};
        DrawTextureEx(midground, mg2Position, 0.0, backgroundScale, WHITE);

        // Draw the foreground
        Vector2 fg1Position = {fgX, 0.0};
        DrawTextureEx(foreground, fg1Position, 0.0, backgroundScale, WHITE);
        Vector2 fg2Position = {fgX + foreground.width * backgroundScale, 0.0};
        DrawTextureEx(foreground, fg2Position, 0.0, backgroundScale, WHITE);

        // Ground check
        if (isOnGround(characterData, WINDOW_HEIGHT))
        {
            // Rectangle is on the ground
            isInAir = false;
            velocity = 0;
        }
        else
        {
            // Rectangle is in the air
            // Apply gravity
            isInAir = true;
            velocity += gravity * deltaTime;
        }

        // Check for jumping
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += JUMP_VELOCITY;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // Update the position of each nebula
            nebulae[i].position.x += nebulaVelocity * deltaTime;
        }

        finishLine += nebulaVelocity * deltaTime;

        // Nebulae
        for (int i = 0; i < sizeOfNebulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], 7, deltaTime);
        }

        // Update character position
        characterData.position.y += velocity * deltaTime;

        if (!isInAir)
        {
            characterData = updateAnimData(characterData, 5, deltaTime);
        }

        for (AnimData nebula : nebulae)
        {
            float pad = 50.0;
            Rectangle nebulaRectangle = {
                nebula.position.x + pad,
                nebula.position.y + pad,
                nebula.rectangle.width - 2 * pad,
                nebula.rectangle.height - 2 * pad,
            };
            Rectangle characterRectangle = {
                characterData.position.x,
                characterData.position.y,
                characterData.rectangle.width,
                characterData.rectangle.height,
            };
            if (CheckCollisionRecs(nebulaRectangle, characterRectangle))
            {
                collision = true;
            }
        }

        if (collision)
        {
            // Lose game
            DrawText("Game Over!", WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 40, WHITE);
        }
        else if (characterData.position.x >= finishLine)
        {
            // Win game
            DrawText("You Win!", WINDOW_WIDTH / 4, WINDOW_HEIGHT / 2, 40, WHITE);
        }
        else
        {
            // Draw nebula
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rectangle, nebulae[i].position, WHITE);
            }

            // Draw character
            DrawTextureRec(character, characterData.rectangle, characterData.position, WHITE);
        }

        EndDrawing();
    }
    UnloadTexture(character);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}