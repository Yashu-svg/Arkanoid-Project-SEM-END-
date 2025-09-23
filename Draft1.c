
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define PLAYER_MAX_LIFE         3
#define LINES_OF_BRICKS         5
#define BRICKS_PER_LINE         10
#define BALLS_MAX               5
#define POWERUPS_MAX            10

typedef enum e_gamestate {
    GAME_TITLE,
    GAME_PLAYING,
    GAME_OVER,
    GAME_WIN
} t_gamestate;

typedef enum e_powerup_type {
    POWERUP_NONE = 0,
    POWERUP_EXPAND,
    POWERUP_EXTRA_LIFE,
    POWERUP_MULTI_BALL
} t_powerup_type;

typedef struct	s_player
{
    Vector2 pos;
    Vector2 size;
    int life;
    float speed;
    bool expanded;
    float expand_timer;
}				t_player;

typedef struct	s_ball
{
    Vector2 pos;
    Vector2 spd;
    float radius;
    bool active;
}				t_ball;

typedef struct	s_brick
{
    Rectangle rect;
    bool active;
}				t_brick;

typedef struct  s_powerup
{
    Vector2 pos;
    Vector2 spd;
    t_powerup_type type;
    bool active;
}               t_powerup;

static const int screenWidth = 960;
static const int screenHeight = 720;

static t_player player = { 0 };
static t_ball balls[BALLS_MAX] = { 0 };
static int ballsCount = 1;
static t_brick bricks[LINES_OF_BRICKS][BRICKS_PER_LINE] = { 0 };
static Vector2 brickSize = { 0 };
static t_gamestate gameState = GAME_TITLE;
static bool pause = false;
static int score = 0;
static t_powerup powerups[POWERUPS_MAX] = { 0 };
static bool waiting_for_launch = true;

void	init_game(void);
void	update_game(void);
void	draw_game(void);
void	unload_game(void);
void	update_draw_frame(void);
void    spawn_powerup(Vector2 pos);
void    apply_powerup(t_powerup_type type);
void    reset_balls(Vector2 pos);

int main(void)
{
    InitWindow(screenWidth, screenHeight, "Arkanoid Hybrid - raylib + 42 style");
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetTargetFPS(60);
    srand((unsigned int)time(0));
    init_game();

    while (!WindowShouldClose())
    {
        if (IsWindowResized()) {
            
        }
        update_draw_frame();
    }

    unload_game();
    CloseWindow();
    return 0;
}

void	init_game(void)
{
   
    brickSize = (Vector2){ screenWidth/(float)BRICKS_PER_LINE, 38 };

   
    player.size = (Vector2){ 140, 22 };
    player.pos = (Vector2){ screenWidth/2.0f - player.size.x/2, screenHeight - 50 };
    player.life = PLAYER_MAX_LIFE;
    player.speed = 8.0f;
    player.expanded = false;
    player.expand_timer = 0.0f;

    
    for (int i = 0; i < BALLS_MAX; i++) balls[i].active = false;
    ballsCount = 1;
    balls[0].radius = 12;
    balls[0].pos = (Vector2){ player.pos.x + player.size.x/2, player.pos.y - balls[0].radius - 2 };
    balls[0].spd = (Vector2){ 0, 0 };
    balls[0].active = false;

    
    for (int y = 0; y < LINES_OF_BRICKS; y++)
    {
        for (int x = 0; x < BRICKS_PER_LINE; x++)
        {
            bricks[y][x].rect = (Rectangle){
                x * brickSize.x + 7,
                y * brickSize.y + 70,
                brickSize.x - 12,
                brickSize.y - 10
            };
            bricks[y][x].active = true;
        }
    }

    
    for (int i = 0; i < POWERUPS_MAX; i++) powerups[i].active = false;

    score = 0;
    pause = false;
    waiting_for_launch = true;
}

void    spawn_powerup(Vector2 pos)
{
    t_powerup_type type = POWERUP_NONE;
    int r = rand()%100;
    if (r < 40) type = POWERUP_EXPAND;
    else if (r < 70) type = POWERUP_EXTRA_LIFE;
    else type = POWERUP_MULTI_BALL;
    for (int i = 0; i < POWERUPS_MAX; i++) {
        if (!powerups[i].active) {
            powerups[i].pos = pos;
            powerups[i].spd = (Vector2){0, 2};
            powerups[i].type = type;
            powerups[i].active = true;
            break;
        }
    }
}

void    apply_powerup(t_powerup_type type)
{
    switch (type) {
        case POWERUP_EXPAND:
            player.expanded = true;
            player.expand_timer = 10.0f;
            player.size.x = 210;
            break;
        case POWERUP_EXTRA_LIFE:
            player.life += 1;
            break;
        case POWERUP_MULTI_BALL:
            for (int i = 0; i < BALLS_MAX && ballsCount < 3; i++) {
                if (balls[i].active) {
                    for (int j = 0; j < BALLS_MAX; j++) {
                        if (!balls[j].active) {
                            balls[j] = balls[i];
                            balls[j].spd.x *= -1;
                            balls[j].spd.y *= (GetRandomValue(0, 1) == 0) ? 1 : -1;
                            balls[j].active = true;
                            ballsCount++;
                            break;
                        }
                    }
                }
            }
            break;
        default: break;
    }
}

void    reset_balls(Vector2 pos)
{
    for (int i = 0; i < BALLS_MAX; i++) balls[i].active = false;
    ballsCount = 1;
    balls[0].radius = 12;
    balls[0].pos = pos;
    balls[0].spd = (Vector2){ 0, 0 };
    balls[0].active = false;
}

void	update_game(void)
{
    if (gameState == GAME_TITLE)
    {
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER))
        {
            init_game();
            gameState = GAME_PLAYING;
        }
    }
    else if (gameState == GAME_PLAYING)
    {
        if (IsKeyPressed(KEY_P)) pause = !pause;

        if (!pause)
        {
            
            if (IsKeyDown(KEY_LEFT)) player.pos.x -= player.speed;
            if (IsKeyDown(KEY_RIGHT)) player.pos.x += player.speed;
            if (player.pos.x < 0) player.pos.x = 0;
            if (player.pos.x + player.size.x > screenWidth) player.pos.x = screenWidth - player.size.x;

            
            if (player.expanded) {
                player.expand_timer -= GetFrameTime();
                if (player.expand_timer <= 0.0f) {
                    player.expanded = false;
                    player.size.x = 140;
                }
            }

            
            if (!balls[0].active)
            {
                balls[0].pos.x = player.pos.x + player.size.x/2;
                balls[0].pos.y = player.pos.y - balls[0].radius - 2;
                if (IsKeyPressed(KEY_SPACE))
                {
                    balls[0].active = true;
                    balls[0].spd = (Vector2){ 3 * ((GetRandomValue(0, 1) == 0) ? -1 : 1), -4 };
                    waiting_for_launch = false;
                }
            }

           
            for (int b = 0; b < BALLS_MAX; b++)
            {
                if (!balls[b].active) continue;

                balls[b].pos.x += balls[b].spd.x;
                balls[b].pos.y += balls[b].spd.y;

               
                if ((balls[b].pos.x - balls[b].radius) <= 0 || (balls[b].pos.x + balls[b].radius) >= screenWidth)
                    balls[b].spd.x *= -1;
                if ((balls[b].pos.y - balls[b].radius) <= 0)
                    balls[b].spd.y *= -1;

               
                Rectangle paddleRect = { player.pos.x, player.pos.y, player.size.x, player.size.y };
                if (CheckCollisionCircleRec(balls[b].pos, balls[b].radius, paddleRect))
                {
                    balls[b].spd.y *= -1;
                    float hitPos = (balls[b].pos.x - (player.pos.x + player.size.x/2)) / (player.size.x/2);
                    balls[b].spd.x = 4 * hitPos;
                }

                
                if ((balls[b].pos.y - balls[b].radius) > screenHeight)
                {
                    balls[b].active = false;
                    ballsCount--;
                }

                
                for (int y = 0; y < LINES_OF_BRICKS; y++)
                {
                    for (int x = 0; x < BRICKS_PER_LINE; x++)
                    {
                        if (bricks[y][x].active && CheckCollisionCircleRec(balls[b].pos, balls[b].radius, bricks[y][x].rect))
                        {
                            bricks[y][x].active = false;
                            balls[b].spd.y *= -1;
                            score += 100;
                            if (GetRandomValue(1,100) <= 22)
                                spawn_powerup((Vector2){bricks[y][x].rect.x + brickSize.x/2, bricks[y][x].rect.y + brickSize.y/2});
                        }
                    }
                }
            }

            
            bool anyBallActive = false;
            for (int b = 0; b < BALLS_MAX; b++)
                if (balls[b].active) anyBallActive = true;

            if (!anyBallActive && !waiting_for_launch)
            {
                player.life--;
                if (player.life <= 0)
                    gameState = GAME_OVER;
                else {
                    reset_balls((Vector2){ player.pos.x + player.size.x/2, player.pos.y - balls[0].radius - 2 });
                    waiting_for_launch = true;
                }
            }

           
            for (int i = 0; i < POWERUPS_MAX; i++)
            {
                if (!powerups[i].active) continue;
                powerups[i].pos.y += powerups[i].spd.y;

               
                Rectangle paddleRect = { player.pos.x, player.pos.y, player.size.x, player.size.y };
                Rectangle puRect = {powerups[i].pos.x-14, powerups[i].pos.y-14, 28, 28};
                if (CheckCollisionRecs(paddleRect, puRect))
                {
                    apply_powerup(powerups[i].type);
                    powerups[i].active = false;
                }
                if (powerups[i].pos.y > screenHeight) powerups[i].active = false;
            }

            
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
            gameState = GAME_TITLE;
        }
    }
}

void	draw_background(void)
{
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
        DrawRectangle(0, i, screenWidth, 4, mid);
    }
}

void    draw_powerup_icon(t_powerup_type type, Vector2 pos)
{
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
        default:
            break;
    }
}

void	draw_title_screen(void)
{
    draw_background();

   
    int arkanoidFontSize = 110;
    int arkanoidWidth = MeasureText("Arkanoid", arkanoidFontSize);
    DrawText("Arkanoid",
        screenWidth/2 - arkanoidWidth/2,
        120,
        arkanoidFontSize,
        (Color){ 255, 180, 60, 255 });

   
    const char *versionStr = "version 1";
    int versionW = MeasureText(versionStr, 28);
    DrawText(versionStr, screenWidth - versionW - 24, screenHeight - 44, 28, LIGHTGRAY);

    
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

void	draw_game(void)
{
    draw_background();

    if (gameState == GAME_TITLE)
    {
        draw_title_screen();
    }
    else if (gameState == GAME_PLAYING)
    {
        DrawRectangleV(player.pos, player.size, player.expanded ? YELLOW : DARKBLUE);
        for (int i = 0; i < player.life; i++)
            DrawRectangle(20 + 44*i, screenHeight - 30, 36, 11, LIGHTGRAY);

        for (int b = 0; b < BALLS_MAX; b++)
            if (balls[b].active)
                DrawCircleV(balls[b].pos, balls[b].radius, MAROON);

        for (int y = 0; y < LINES_OF_BRICKS; y++)
            for (int x = 0; x < BRICKS_PER_LINE; x++)
                if (bricks[y][x].active)
                    DrawRectangleRec(bricks[y][x].rect, (y + x) % 2 ? GRAY : ORANGE);

        for (int i = 0; i < POWERUPS_MAX; i++)
            if (powerups[i].active)
                draw_powerup_icon(powerups[i].type, powerups[i].pos);

        DrawText(TextFormat("SCORE: %04i", score), screenWidth - 170, 20, 28, DARKGRAY);

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

void	unload_game(void)
{
   
}

void	update_draw_frame(void)
{
    update_game();
    BeginDrawing();
    draw_game();
    EndDrawing();

}
