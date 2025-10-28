/**
* Author: Nicolas Ollivier
* Assignment: Lunar Lander
* Date due: 2025-10-27, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/Entity.h"

struct GameState
{
    Entity *rockey;
    Entity *ufo;
    Map *map;
};

// Global Constants
constexpr int SCREEN_WIDTH  = 1000,
              SCREEN_HEIGHT = 600,
              FPS           = 120;

constexpr char    BG_COLOUR[]      = "#000000ff";
constexpr Vector2 ORIGIN           = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 },
                  ATLAS_DIMENSIONS = { 6, 8 };

constexpr float TILE_DIMENSION          = 40.0f,
                // in m/ms², since delta time is in ms
                ACCELERATION_OF_GRAVITY = 10.0f,
                FIXED_TIMESTEP          = 1.0f / 60.0f,
                END_GAME_THRESHOLD      = 800.0f,
                ALIEN_X                 = 300.0f;

constexpr int LEVEL_WIDTH  = 30,
              LEVEL_HEIGHT = 15;
constexpr unsigned int LEVEL_DATA[] = {
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1,
   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 3, 3, 3, 3, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 3, 0, 4, 0, 0, 3, 3, 3, 3, 3, 0, 0, 3, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 3, 0, 4, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 3, 0, 4, 0, 0, 3, 3, 3, 3, 3, 0, 0, 0, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 2, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 3, 3, 4, 0, 0, 0, 0, 4, 4, 4, 3, 3, 3, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 3, 3, 4, 0, 0, 0, 0, 4, 4, 4, 3, 3, 3, 1, 1,
   1, 1, 0, 4, 0, 0, 4, 4, 0, 4, 0, 0, 0, 4, 3, 3, 3, 3, 3, 3, 0, 0, 4, 4, 4, 3, 3, 3, 1, 1,
   1, 1, 0, 4, 4, 0, 4, 4, 0, 4, 4, 0, 0, 4, 3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 3, 3, 1, 1,
   1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 0, 3, 3, 3, 3, 3, 1, 1,
};

// Global Variables
AppStatus gAppStatus   = RUNNING;
float gPreviousTicks   = 0.0f,
      gTimeAccumulator = 0.0f;

GameState gState;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maps");
    /*
        ----------- MAP -----------
    */
    gState.map = new Map(
        LEVEL_WIDTH, LEVEL_HEIGHT,   // map grid cols & rows
        (unsigned int *) LEVEL_DATA, // grid data
        "assets/game/tilesheet.png", // texture filepath
        TILE_DIMENSION,              // tile size
        4, 1,                        // texture cols & rows
        ORIGIN                       // in-game origin
    );

    /*
        ----------- PROTAGONIST -----------
    */
    float sizeRatio  = 48.0f / 64.0f;

    // Assets from @see https://sscary.itch.io/the-adventurer-female
    gState.rockey = new Entity(
        {ORIGIN.x - 300.0f, ORIGIN.y - 200.0f},     // position
        {TILE_DIMENSION, TILE_DIMENSION},           // scale
        "assets/game/rockey.png",                   // texture file address
        PLAYER                                      // entity type
    );

    gState.ufo = new Entity(
        {ALIEN_X, ORIGIN.y},                       // position
        {TILE_DIMENSION*2.0f, TILE_DIMENSION*2.0f}, // scale
        "assets/game/UFO.png",                      // texture file address
        UFO                                         // entity type
    );

    gState.rockey->setColliderDimensions({
        gState.rockey->getScale().x,
        gState.rockey->getScale().y 
    });
    gState.rockey->setAcceleration({0.0f, ACCELERATION_OF_GRAVITY});

    SetTargetFPS(FPS);
}

void processInput() 
{
    gState.rockey -> resetMovement();
    gState.rockey -> setAcceleration({0, ACCELERATION_OF_GRAVITY});

    if      (IsKeyDown(KEY_A)) gState.rockey->rotateLeft();
    else if (IsKeyDown(KEY_D)) gState.rockey->rotateRight();

    if (IsKeyDown(KEY_W))
    {
        gState.rockey->boost();
    }

    // if (GetLength(gState.rockey->getMovement()) > 1.0f) 
    //     gState.rockey->normaliseMovement();

    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    if (gState.rockey->isGameOver()){
        return; // Don't update if game is over
    }
    // Delta time
    float ticks = (float) GetTime();
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks  = ticks;

    // Fixed timestep
    deltaTime += gTimeAccumulator;

    if (deltaTime < FIXED_TIMESTEP)
    {
        gTimeAccumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        gState.rockey->update(
            FIXED_TIMESTEP, // delta time / fixed timestep
            nullptr,        // player
            gState.map,     // map
            gState.ufo,     // collidable entities
            1               // col. entity count
        );

        deltaTime -= FIXED_TIMESTEP;

        Vector2 currentPlayerPosition = { gState.rockey->getPosition().x, ORIGIN.y };
        Vector2 alienPosition = gState.ufo->getPosition();

        // Simple moving platform
        gState.ufo->setPosition({
            ALIEN_X,
            ORIGIN.y + sin(ticks)*20.0f,
        });
        if (gState.rockey->getPosition().y > 800.0f) gAppStatus = TERMINATED;
    }
}

void render()
{
    BeginDrawing();
    ClearBackground(ColorFromHex(BG_COLOUR));

    gState.rockey->render();
    gState.ufo->render();
    gState.map->render();
    DrawText(TextFormat("Fuel: %.2f", gState.rockey->getFuel()), 100, 80, 20, RED);

    if (gState.rockey->isGameOver() && gState.rockey->hasWon()){
        DrawText(TextFormat("Mission Accomplished"), 100, ORIGIN.y-50, 50, GREEN);
    }
    else if (gState.rockey->isGameOver() && gState.rockey->hasLost()){
        DrawText(TextFormat("Mission Failed"), 100, ORIGIN.y-50, 50, RED);
    }

    EndDrawing();
}

void shutdown() 
{
    delete gState.rockey;
    delete gState.map;

    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}