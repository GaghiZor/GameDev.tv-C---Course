#include "raylib.h"

int main()
{
    // Window dimensions
    int windowWidth{800};
    int windowHeight{450};
    InitWindow(windowWidth, windowHeight, "My first Window in RayLib");

    // Circle coordinates
    int circleX = 200;
    int circleY = 200;
    int circleRadius = 25;

    // Circle edges
    int leftCircleX = circleX - circleRadius;
    int rightCircleX = circleX + circleRadius;
    int topCircleY = circleY - circleRadius;
    int bottomCircleY = circleY + circleRadius;

    // Axe coordinates
    int axeX = 400;
    int axeY = 0;
    int axeLenght = 50;

    // Axe edges
    int leftAxeX = axeX;
    int rightAxeX = axeX + axeLenght;
    int topAxeY = axeY;
    int bottomAxeY = axeY + axeLenght;

    bool collisionDetected = ((bottomAxeY >= topCircleY) &&
                              (topAxeY <= bottomCircleY) &&
                              (rightAxeX >= leftCircleX) &&
                              (leftAxeX <= rightCircleX));

    int direction = 10;

    SetTargetFPS(60);
    while (WindowShouldClose() == false)
    {
        BeginDrawing();
        ClearBackground(BLACK);

        if (collisionDetected)
        {
            DrawText("Game Over!", 400, 200, 20, RED);
        }
        else
        {
            // Game logic begins

            // Update the edges
            leftCircleX = circleX - circleRadius;
            rightCircleX = circleX + circleRadius;
            topCircleY = circleY - circleRadius;
            bottomCircleY = circleY + circleRadius;
            leftAxeX = axeX;
            rightAxeX = axeX + axeLenght;
            topAxeY = axeY;
            bottomAxeY = axeY + axeLenght;

            // Update collisionDetected
            collisionDetected = ((bottomAxeY >= topCircleY) &&
                                 (topAxeY <= bottomCircleY) &&
                                 (rightAxeX >= leftCircleX) &&
                                 (leftAxeX <= rightCircleX));

            DrawCircle(circleX, circleY, circleRadius, GREEN);
            DrawRectangle(axeX, axeY, axeLenght, axeLenght, RED);

            // Move axe
            axeY += direction;
            if (axeY >= windowHeight - axeLenght || axeY <= 0)
            {
                direction = -direction;
            }

            if (IsKeyDown(KEY_D) && circleX < windowWidth - circleRadius)
            {
                circleX += 5;
            }
            if (IsKeyDown(KEY_A) && circleX > 0 + circleRadius)
            {
                circleX -= 5;
            }
            // Game logic ends
        }

        EndDrawing();
    }
}