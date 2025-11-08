/*******************************************************************************************
*
*   Arkanoid Hybrid - raylib + 42-style structure (with detailed line-by-line comments)
*
********************************************************************************************/

#include "raylib.h"                  // Loads raylib library for graphics, windows, and input
#include <stdio.h>                   // Standard I/O library for debugging (optional here)
#include <stdlib.h>                  // Standard library for things like random numbers
#include <math.h>                    // Math functions, used mostly for collision/math ops
#include <time.h>                    // Needed for random seed initialization

//----------------------------------------------------------------------------------
// Defines - #define macros for tuneable numbers, easy tweaking
//----------------------------------------------------------------------------------
#define PLAYER_MAX_LIFE    3         // Maximum number of lives player starts with
#define LINES_OF_BRICKS    5         // Number of rows of bricks in the game area
#define BRICKS_PER_LINE    10        // Number of bricks per row
#define BALLS_MAX          5         // Maximum number of balls that can exist at a time
#define POWERUPS_MAX       10        // Maximum number of falling powerup objects

//----------------------------------------------------------------------------------
// Enumerations - allows readable states and powerup types
//----------------------------------------------------------------------------------
typedef enum e_gamestate {
    GAME_TITLE,                      // Title screen shown
    GAME_PLAYING,                    // Main gameplay
    GAME_OVER,                       // Game Over screen shown
    GAME_WIN                         // Victory/Win screen shown
} t_gamestate;

typedef enum e_powerup_type {
    POWERUP_NONE = 0,                // Value for "no powerup"
    POWERUP_EXPAND,                  // Expands the paddle if collected
    POWERUP_EXTRA_LIFE,              // Gives player 1 extra life
    POWERUP_MULTI_BALL               // Splits ball into more balls
} t_powerup_type;

//----------------------------------------------------------------------------------
// Structure Definitions - represents major game "objects"
//----------------------------------------------------------------------------------
typedef struct s_player
{
    Vector2 pos;                     // Player paddle's position (x, y on screen)
    Vector2 size;                    // Paddle's size (width, height)
    int life;                        // Number of remaining lives
    float speed;                     // Paddle movement speed (pixels per frame)
    bool expanded;                   // If paddle is currently "expanded" or not
    float expand_timer;              // Time left for expanded paddle effect
} t_player;

typedef struct s_ball
{
    Vector2 pos;                     // Ball position (center x, y)
    Vector2 spd;                     // Ball speed vector (x, y delta per frame)
    float radius;                    // Ball radius (size)
    bool active;                     // Is the ball in play/moving (true) or at rest (false)
} t_ball;

typedef struct s_brick
{
    Rectangle rect;                  // Rectangle for brick position/size
    bool active;                     // Is brick still visible (true), or destroyed (false)
} t_brick;

typedef struct s_powerup
{
    Vector2 pos;                     // Powerup position (center x, y)
    Vector2 spd;                     // Powerup falling speed (y only)
    t_powerup_type type;             // Which powerup this is (expand, life, multi-ball)
    bool active;                     // Is powerup still falling/visible
} t_powerup;

//------------------------------------------------------------------------------------
// Global Variables - accessible everywhere in file for game state
//------------------------------------------------------------------------------------
static const int screenWidth = 960;         // Game window pixel width
static const int screenHeight = 720;        // Game window pixel height

static t_player player = { 0 };             // One player struct, initialized to all zeros
static t_ball balls[BALLS_MAX] = { 0 };     // Array of all possible balls (max BALLS_MAX)
static int ballsCount = 1;                  // Current number of balls active/in play
static t_brick bricks[LINES_OF_BRICKS][BRICKS_PER_LINE] = { 0 }; // 2D array of bricks
static Vector2 brickSize = { 0 };           // Size of each brick calculated at runtime
static t_gamestate gameState = GAME_TITLE;  // Overall game state (starts at title screen)
static bool pause = false;                  // Is the game currently paused?
static int score = 0;                       // Player score (starts at 0)
static t_powerup powerups[POWERUPS_MAX] = { 0 }; // Array of possible falling powerups
static bool waiting_for_launch = true;      // Between life loss and ball ready for relaunch

//------------------------------------------------------------------------------------
// Function Prototypes - tells compiler what functions exist below
//------------------------------------------------------------------------------------
void   init_game(void);                    // Sets up all game variables for new game/start
void   update_game(void);                  // Steps game logic according to game state
void   draw_game(void);                    // Draws all objects depending on game state
void   unload_game(void);                  // (Unused for now) To clean up any resources
void   update_draw_frame(void);            // Calls update/draw per frame
void   spawn_powerup(Vector2 pos);         // Creates a powerup object at brick coords
void   apply_powerup(t_powerup_type type); // Applies effect of collected powerup
void   reset_balls(Vector2 pos);           // Resets all balls after loss

//------------------------------------------------------------------------------------
// Main Entry Point
//------------------------------------------------------------------------------------
int main(void)
{
    InitWindow(screenWidth, screenHeight, "Arkanoid "); // Creates window
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); // Resizable window & VSync to cap FPS
    SetTargetFPS(60);                                        // Runs at 60 frames/second
    srand((unsigned int)time(0));                            // Seeds RNG for randomness
    init_game();                                             // Sets up all variables and objects

    while (!WindowShouldClose())                             // Main game loop; exits when window closes
    {
        if (IsWindowResized()) {
            // Optional: keep brick sizes and paddle scaling fixed, or readjust as desired
        }
        update_draw_frame();                                 // Updates and draws this frame
    }

    unload_game();                                           // Call cleanup (if needed)
    CloseWindow();                                           // Close window and terminate
    return 0;                                                // Exit with code 0 (success)
}

//------------------------------------------------------------------------------------
// Module Functions - major building blocks
//------------------------------------------------------------------------------------
void init_game(void)
{
    // Calculate brick size based on screen width and number of bricks per line
    brickSize = (Vector2){ screenWidth/(float)BRICKS_PER_LINE, 38 };

    // Initialize player (paddle)
    player.size = (Vector2){ 140, 22 };                          // Paddle width/height
    player.pos = (Vector2){ screenWidth/2.0f - player.size.x/2, screenHeight - 50 }; // Center & offset paddle near bottom
    player.life = PLAYER_MAX_LIFE;                               // Set lives to max value
    player.speed = 8.0f;                                         // Set left/right paddle movement speed
    player.expanded = false;                                     // Not expanded at start
    player.expand_timer = 0.0f;                                  // No expansion timer at start

    // Initialize balls
    for (int i = 0; i < BALLS_MAX; i++) balls[i].active = false; // All balls inactive at first
    ballsCount = 1;                                              // Start with only 1 ball available
    balls[0].radius = 12;                                        // Standard ball size
    balls[0].pos = (Vector2){ player.pos.x + player.size.x/2, player.pos.y - balls[0].radius - 2 }; // Ball starts above paddle
    balls[0].spd = (Vector2){ 0, 0 };                            // Ball doesn't move until launched
    balls[0].active = false;                                     // Ball at rest initially

    // Initialize bricks
    for (int y = 0; y < LINES_OF_BRICKS; y++)
    {
        for (int x = 0; x < BRICKS_PER_LINE; x++)
        {
            bricks[y][x].rect = (Rectangle){
                x * brickSize.x + 7,                    // X position (with left margin)
                y * brickSize.y + 70,                   // Y position (with top margin)
                brickSize.x - 12,                       // Brick width (with padding)
                brickSize.y - 10                        // Brick height (with padding)
            };
            bricks[y][x].active = true;                 // Mark all bricks as visible and undestroyed
        }
    }

    // Initialize powerups
    for (int i = 0; i < POWERUPS_MAX; i++) powerups[i].active = false; // All powerups start inactive

    score = 0;                   // Reset score
    pause = false;               // Unpause if previously paused
    waiting_for_launch = true;   // Ball ready to be launched (space bar)
}

void spawn_powerup(Vector2 pos)
{
    t_powerup_type type = POWERUP_NONE;                 // Default powerup type
    int r = rand() % 100;                               // Get random value 0-99
    if (r < 40) type = POWERUP_EXPAND;
    else if (r < 70) type = POWERUP_EXTRA_LIFE;
    else type = POWERUP_MULTI_BALL;
    for (int i = 0; i < POWERUPS_MAX; i++) {            // Find a slot for new powerup
        if (!powerups[i].active) {                      // Only spawn if inactive
            powerups[i].pos = pos;                      // Set position
            powerups[i].spd = (Vector2){0, 2};          // Fall straight down at speed 2
            powerups[i].type = type;                    // Set powerup type
            powerups[i].active = true;                  // Mark as active/visible
            break;
        }
    }
}

void apply_powerup(t_powerup_type type)
{
    switch (type) {
        case POWERUP_EXPAND:                             // Expand paddle powerup
            player.expanded = true;
            player.expand_timer = 10.0f;                 // Lasts 10 seconds
            player.size.x = 210;                         // Increase paddle width
            break;
        case POWERUP_EXTRA_LIFE:                         // Extra life powerup
            player.life += 1;                            // Give one more life
            break;
        case POWERUP_MULTI_BALL:                         // Multi-ball powerup
            for (int i = 0; i < BALLS_MAX && ballsCount < 3; i++) {
                if (balls[i].active) {                   // For every active ball
                    for (int j = 0; j < BALLS_MAX; j++) {
                        if (!balls[j].active) {          // Find an inactive slot
                            balls[j] = balls[i];         // Clone ball properties
                            balls[j].spd.x *= -1;        // Reverse ball's X to split
                            balls[j].spd.y *= (GetRandomValue(0, 1) == 0) ? 1 : -1; // Randomize split direction
                            balls[j].active = true;
                            ballsCount++;                // Increase ball count
                            break;
                        }
                    }
                }
            }
            break;
        default: break;                                  // If no powerup, do nothing
    }
}

void reset_balls(Vector2 pos)
{
    for (int i = 0; i < BALLS_MAX; i++) balls[i].active = false; // Deactivate all balls
    ballsCount = 1;                      // Only have one ball after reset
    balls[0].radius = 12;                // Standard size
    balls[0].pos = pos;                  // Place at given position
    balls[0].spd = (Vector2){ 0, 0 };    // Ball at rest
    balls[0].active = false;             // Wait for launch
}

void update_game(void)
{
    if (gameState == GAME_TITLE)
    {
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
        {
            init_game();                 // Start new game on space/enter
            gameState = GAME_PLAYING;    // Switch to gameplay
        }
    }
    else if (gameState == GAME_PLAYING)
    {
        if (IsKeyPressed(KEY_P)) pause = !pause;     // Toggle pause with P key

        if (!pause)                                 // Updates only if not paused
        {
            // -------- Player movement (left/right arrow keys) --------
            if (IsKeyDown(KEY_LEFT)) player.pos.x -= player.speed;
            if (IsKeyDown(KEY_RIGHT)) player.pos.x += player.speed;
            if (player.pos.x < 0) player.pos.x = 0;  // Prevent going off left edge
            if (player.pos.x + player.size.x > screenWidth) player.pos.x = screenWidth - player.size.x; // Prevent right edge

            // -------- Shrink paddle if "expand" timer runs out --------
            if (player.expanded) {
                player.expand_timer -= GetFrameTime();      // Subtract elapsed frame time
                if (player.expand_timer <= 0.0f) {
                    player.expanded = false;                // Effect over
                    player.size.x = 140;                    // Reset paddle size
                }
            }

            // -------- Launch ball (before launch, it sticks to paddle) --------
            if (!balls[0].active)
            {
                balls[0].pos.x = player.pos.x + player.size.x/2;
                balls[0].pos.y = player.pos.y - balls[0].radius - 2;
                if (IsKeyPressed(KEY_SPACE))
                {
                    balls[0].active = true;                        // Set moving
                    balls[0].spd = (Vector2){
                        7 * ((GetRandomValue(0, 1) == 0) ? -1 : 1), // Speed x: left/right random
                        -7 };                                       // Speed y: always up at start
                    waiting_for_launch = false;
                }
            }

            // -------- Ball logic for all balls (movement, collisions, etc) --------
            for (int b = 0; b < BALLS_MAX; b++)
            {
                if (!balls[b].active) continue;                     // Only process active balls

                balls[b].pos.x += balls[b].spd.x;                   // Move ball by speed
                balls[b].pos.y += balls[b].spd.y;

                // ----- Collision with left/right walls -----
                if ((balls[b].pos.x - balls[b].radius) <= 0 || (balls[b].pos.x + balls[b].radius) >= screenWidth)
                    balls[b].spd.x *= -1;
                // ----- Collision with top wall -----
                if ((balls[b].pos.y - balls[b].radius) <= 0)
                    balls[b].spd.y *= -1;

                // ----- Collision with paddle -----
                Rectangle paddleRect = { player.pos.x, player.pos.y, player.size.x, player.size.y };
                if (CheckCollisionCircleRec(balls[b].pos, balls[b].radius, paddleRect))
                {
                    balls[b].spd.y *= -1;                                 // Bounce ball away
                    float hitPos = (balls[b].pos.x - (player.pos.x + player.size.x/2)) / (player.size.x/2);
                    balls[b].spd.x = 6 * hitPos;                          // Adjust angle based on hit position
                }

                // ----- Ball missed (falls below screen) -----
                if ((balls[b].pos.y - balls[b].radius) > screenHeight)
                {
                    balls[b].active = false;                              // Remove ball
                    ballsCount--;
                }

                // ----- Collision with bricks -----
                for (int y = 0; y < LINES_OF_BRICKS; y++)
                {
                    for (int x = 0; x < BRICKS_PER_LINE; x++)
                    {
                        if (bricks[y][x].active &&
                            CheckCollisionCircleRec(balls[b].pos, balls[b].radius, bricks[y][x].rect))
                        {
                            bricks[y][x].active = false;                  // Destroy brick
                            balls[b].spd.y *= -1;                         // Bounce ball
                            score += 100;                                 // Add score
                            if (GetRandomValue(1,100) <= 22)              // ~22% chance to spawn powerup
                                spawn_powerup((Vector2){
                                    bricks[y][x].rect.x + brickSize.x/2,
                                    bricks[y][x].rect.y + brickSize.y/2
                                });
                        }
                    }
                }
            }

            // -------- Lose life if all balls lost (only after launch) --------
            bool anyBallActive = false;
            for (int b = 0; b < BALLS_MAX; b++)
                if (balls[b].active) anyBallActive = true;

            if (!anyBallActive && !waiting_for_launch)
            {
                player.life--;
                if (player.life <= 0)
                    gameState = GAME_OVER;                             // End game
                else {
                    reset_balls((Vector2){                            // Set up next ball for launching above paddle
                        player.pos.x + player.size.x/2,
                        player.pos.y - balls[0].radius - 2
                    });
                    waiting_for_launch = true;
                }
            }

            // -------- Powerup falling and collection logic --------
            for (int i = 0; i < POWERUPS_MAX; i++)
            {
                if (!powerups[i].active) continue;
                powerups[i].pos.y += powerups[i].spd.y;                 // Fall down

                // Paddle collects powerup
                Rectangle paddleRect = { player.pos.x, player.pos.y, player.size.x, player.size.y };
                Rectangle puRect = {powerups[i].pos.x-14, powerups[i].pos.y-14, 28, 28};
                if (CheckCollisionRecs(paddleRect, puRect))
                {
                    apply_powerup(powerups[i].type);                    // Apply effect
                    powerups[i].active = false;
                }
                if (powerups[i].pos.y > screenHeight) powerups[i].active = false; // Offscreen cleanup
            }

            // -------- Check win condition (no bricks left) --------
            bool bricksLeft = false;
            for (int y = 0; y < LINES_OF_BRICKS; y++)
                for (int x = 0; x < BRICKS_PER_LINE; x++)
                    if (bricks[y][x].active) bricksLeft = true;
            if (!bricksLeft) gameState = GAME_WIN;
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        {
            gameState = GAME_TITLE;                        // Return to title on key
        }
    }
}

void draw_background(void)
{
    // Vertical gradient background fill
    for (int i = 0; i < screenHeight; i += 4)
    {
        float t = (float)i / screenHeight;
        Color top = (Color){ 40, 40, 90, 255 };
        Color bot = (Color){ 130, 130, 220, 255 };
        Color mid = (Color){
            (int)(top.r * (1-t) + bot.r * t),
            (int)(top.g * (1-t) + bot.g * t),
            (int)(top.b * (1-t) + bot.b * t),
            255
        };
        DrawRectangle(0, i, screenWidth, 4, mid);              // 4-pixel-thick horizontal stripe
    }
}

void draw_powerup_icon(t_powerup_type type, Vector2 pos)
{
    // Draws graphical representation of powerup, based on type
    switch (type)
    {
        case POWERUP_EXPAND:
            DrawRectangle((int)pos.x-12,(int)pos.y-7,24,14, YELLOW);
            DrawRectangleLines((int)pos.x-12,(int)pos.y-7,24,14, BLACK);
            DrawText("E", (int)pos.x-6, (int)pos.y-7, 16, BLACK);
            break;
        case POWERUP_EXTRA_LIFE:
            DrawCircle((int)pos.x, (int)pos.y, 12, RED);
            DrawText("+", (int)pos.x-6, (int)pos.y-12, 22, WHITE);
            break;
        case POWERUP_MULTI_BALL:
            DrawCircle((int)pos.x-7, (int)pos.y, 7, MAROON);
            DrawCircle((int)pos.x+7, (int)pos.y, 7, MAROON);
            DrawCircle((int)pos.x, (int)pos.y, 7, MAROON);
            break;
        default: break;
    }
}

void draw_title_screen(void)
{
    draw_background();                                     // Fill window with background gradient

    // Draw "Arkanoid" as main title
    int arkanoidFontSize = 110;                            // Size for main title text
    int arkanoidWidth = MeasureText("Arkanoid", arkanoidFontSize);
    DrawText("Arkanoid",
        screenWidth/2 - arkanoidWidth/2,                   // Center horizontally
        120,                                               // Y position for title
        arkanoidFontSize,
        (Color){ 255, 180, 60, 255 });

    // Version info at bottom right
    const char *versionStr = "version 1";
    int versionW = MeasureText(versionStr, 28);
    DrawText(versionStr, screenWidth - versionW - 24, screenHeight - 44, 28, LIGHTGRAY);

    // Control and powerup instructions below title
    const char *instructions[] = {
        "Press SPACE or ENTER to start",
        "Move paddle: LEFT / RIGHT arrow keys",
        "Launch ball: SPACE",
        "Pause/Resume: P",
        "Clear all bricks to win!",
        "",
        "Powerups:",
        "   E = Expand Paddle,   + = Extra Life,   Three Balls = Multi-ball"
    };
    int instrStartY = 290;
    for (int i = 0; i < 8; i++) {
        int instrW = MeasureText(instructions[i], 26);
        DrawText(instructions[i],
            screenWidth/2 - instrW/2,
            instrStartY + i*32,
            26, RAYWHITE);
    }
}

void draw_game(void)
{
    draw_background();    // Draw background for all states

    if (gameState == GAME_TITLE)
    {
        draw_title_screen();
    }
    else if (gameState == GAME_PLAYING)
    {
        // Draw paddle (expanded color if effect active)
        DrawRectangleV(player.pos, player.size, player.expanded ? YELLOW : DARKBLUE);
        // Draw life rectangles at bottom left
        for (int i = 0; i < player.life; i++)
            DrawRectangle(20 + 44*i, screenHeight - 30, 36, 11, LIGHTGRAY);

        // Draw balls
        for (int b = 0; b < BALLS_MAX; b++)
            if (balls[b].active)
                DrawCircleV(balls[b].pos, balls[b].radius, MAROON);

        // Draw bricks
        for (int y = 0; y < LINES_OF_BRICKS; y++)
            for (int x = 0; x < BRICKS_PER_LINE; x++)
                if (bricks[y][x].active)
                    DrawRectangleRec(bricks[y][x].rect, (y + x) % 2 ? GRAY : ORANGE);

        // Draw powerups
        for (int i = 0; i < POWERUPS_MAX; i++)
            if (powerups[i].active)
                draw_powerup_icon(powerups[i].type, powerups[i].pos);

        // Draw score at top right
        DrawText(TextFormat("SCORE: %04i", score), screenWidth - 170, 20, 28, YELLOW);

        // Draw "PAUSED" overlay
        if (pause)
            DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 48)/2, screenHeight/2 - 48, 48, GRAY);
    }
    else if (gameState == GAME_OVER)
    {
        DrawText("GAME OVER", screenWidth/2 - MeasureText("GAME OVER", 56)/2, screenHeight/2 - 80, 56, RED);
        DrawText(TextFormat("FINAL SCORE: %i", score), screenWidth/2-MeasureText("FINAL SCORE: 0000", 32)/2, screenHeight/2, 32, MAROON);
        DrawText("PRESS [ENTER] TO RETURN TO TITLE", screenWidth/2-MeasureText("PRESS [ENTER] TO RETURN TO TITLE", 26)/2, screenHeight/2 + 72, 26, DARKGRAY);
    }
    else if (gameState == GAME_WIN)
    {
        DrawText("VICTORY!", screenWidth/2 - MeasureText("VICTORY!", 64)/2, screenHeight/2 - 96, 64, DARKGREEN);
        DrawText(TextFormat("FINAL SCORE: %i", score), screenWidth/2-MeasureText("FINAL SCORE: 0000", 34)/2, screenHeight/2, 34, MAROON);
        DrawText("YOU CLEARED ALL THE BRICKS!", screenWidth/2-MeasureText("YOU CLEARED ALL THE BRICKS!", 28)/2, screenHeight/2 + 48, 28, ORANGE);
        DrawText("PRESS [ENTER] TO RETURN TO TITLE", screenWidth/2-MeasureText("PRESS [ENTER] TO RETURN TO TITLE", 26)/2, screenHeight/2 + 96, 26, DARKGRAY);
    }
}

void unload_game(void)
{
    // Currently unused. Could clean up loaded assets or resources if needed.
}

void update_draw_frame(void)
{
    update_game();   // Step logic for one frame
    BeginDrawing();  // Begin rendering
    draw_game();     // Draw everything for one frame
    EndDrawing();    // End rendering
}