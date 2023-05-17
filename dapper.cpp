#include "raylib.h"
#include <iostream>

using namespace std;

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimFrame(AnimData data, float deltaTime, int maxFrame)
{

    // update scarfy running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {

        data.runningTime = 0.0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
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

    // window dimennsions
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    // initialize the window
    InitWindow(windowDimensions[0], windowDimensions[1], "Dapper Dasher!");

    // acceleration due to gravity (pixels per s)/ s
    const int gravity{100};

    // loading nebula
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    const int sizeOfNebulae{6};
    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
    {
        nebulae[i].rec = {0.0, 0.0, nebula.width / 8, nebula.height / 8};
        nebulae[i].pos.y = windowDimensions[1] - (nebula.height / 8);
        nebulae[i].frame = 0.0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 1.0 / 16.0;
        nebulae[i].pos.x = windowDimensions[0] + (300 * i);
    }

    float finishLine{nebulae[sizeOfNebulae - 1].pos.x};

    // nebula x velocity (pixels per second)
    int nebVel{-150};

    // loading scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");

    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;

    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;

    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0 / 2.0;
    scarfyData.runningTime = 0.0;

    // velocity of scarfy
    int velocity{0};

    // jump velocity pixel per second
    const int jumpVel{-8500};

    // is scarfy in the air
    bool isInAir{false};

    Texture2D background = LoadTexture("textures/far-buildings.png");

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    Texture2D foreground = LoadTexture("textures/foreground.png");

    float bgX{0.0};
    float mgX{0.0};
    float fgX(0.0);

    bool collision{false};

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        // delta time
        const float dT{GetFrameTime()};

        bgX -= 20 * dT;

        if (bgX <= -background.width * 2)
        {
            bgX = 0.0;
        }

        mgX -= 40 * dT;

        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0;
        }

        fgX -= 80 * dT;

        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos{bgX + background.width * 2, 0.0};

        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw a midground
        Vector2 mg1Pos{mgX, 0.0};
        Vector2 mg2Pos{mgX + midground.width * 2, 0.0};

        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw a foreground
        Vector2 fg1Pos{fgX, 0.0};
        Vector2 fg2Pos{fgX + foreground.width * 2, 0.0};

        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        if (isOnGround(scarfyData, windowDimensions[1]))
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            velocity = (velocity * dT) + gravity;
            isInAir = true;
        }

        // if space is pressed jump
        if (IsKeyDown(KEY_SPACE) && isInAir == false)
        {
            velocity += jumpVel;
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update nebula positon
            nebulae[i].pos.x += nebVel * dT;
        }

        finishLine += nebVel * dT;

        // update scarfy positon
        scarfyData.pos.y += velocity * dT;

        // update scarfys animation frame
        if (!isInAir)
        {

            scarfyData = updateAnimFrame(scarfyData, dT, 5);
        }

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            // update nebula animation frame
            nebulae[i] = updateAnimFrame(nebulae[i], dT, 7);
        }

        for (AnimData nebula : nebulae)
        {

            float pad{30};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad};

            Rectangle scarfyRec{
                scarfyData.pos.x + pad,
                scarfyData.pos.y + pad,
                scarfyData.rec.width - 2 * pad,
                scarfyData.rec.height - 2 * pad};
            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        if (collision)
        {
            DrawText("Game Over", windowDimensions[0] / 4, windowDimensions[1] / 2, 50, RED);
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0] / 4, windowDimensions[1] / 2, 50, GREEN);
        }
        else
        {
            // update scarfy running time
            scarfyData.runningTime += scarfyData.runningTime + dT;

            for (int i = 0; i < sizeOfNebulae; i++)
            {
                // draw nebula
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }

            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        // stop drawing
        EndDrawing();
    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);

    CloseWindow();
}