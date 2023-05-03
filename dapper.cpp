#include "raylib.h"
#include <iostream>

using namespace std;

int main()
{

    // window dimennsions
    const int windowWidth{800};
    const int windowHeight{600};
    // initialize the window
    InitWindow(windowWidth, windowHeight, "Dapper Dasher!");

    int velocity{0};

    // jump velocity pixel per second
    const int jumpVel{-4000};

    // acceleration due to gravity (pixels per s)/ s
    const int gravity{100};

    //loading nebula
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    // loading scarfy
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    
    Vector2 scarfyPos;
    scarfyPos.x =windowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;


    // animation frame
    int frame{0};


    // is the rectangle in the air
    bool isInAir{false};

    // how much shoudl be the time between animation update
    const float updateTime{1.0/2.0};
    
    
    // we know how much time has passed since last update time
    float runningTime{0};



    SetTargetFPS(60);

    while (!WindowShouldClose())
    {

        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);
        const float dT{GetFrameTime()};

        if (scarfyPos.y >= windowHeight - scarfy.height)
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            velocity = (velocity * dT) + gravity ; 
            isInAir = true;
        }

        if (IsKeyDown(KEY_SPACE) && isInAir == false)
        {
            velocity += jumpVel;
        }

        scarfyPos.y += velocity * dT;
        
        // update running time
        runningTime += runningTime + dT;


        if (runningTime >= updateTime){

            runningTime = 0;

            //update animation frame
            scarfyRec.x = frame * scarfyRec.width;
            frame++;
            if (frame > 5){
                frame = 0;
            }
        }

        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        // stop drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
 
    CloseWindow();
}