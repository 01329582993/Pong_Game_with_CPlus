#include <iostream>
#include <raylib.h>

using namespace std;

int player_score = 0;
int cpu_score = 0;

class Ball
{
public:
    float x_ball, y_ball;
    int speed_x, speed_y;
    int radius;

    void Draw()
    {
        DrawCircle(x_ball, y_ball, radius, YELLOW);
    }

    void Update()
    {
        x_ball += speed_x;
        y_ball += speed_y;

        if (y_ball + radius >= GetScreenHeight() || y_ball - radius <= 0)
        {
            speed_y *= -1;
        }

        if (x_ball + radius >= GetScreenWidth())
        {
            cpu_score++;
            ResetBall();
        }
        if (x_ball - radius <= 0)
        {
            player_score++;
            ResetBall();
        }
    }

    void ResetBall()
    {
        x_ball = GetScreenWidth()/2;
        y_ball = GetScreenHeight()/2; 

        int speed_choices[2]= {-1, 1};

        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];
    }
};

class Paddle
{
protected:
    void LimitMovement()
    {
        if (y_paddle <= 0)
        {
            y_paddle = 0;
        }
        if (y_paddle + height_paddle >= GetScreenHeight())
        {
            y_paddle = GetScreenHeight() - height_paddle;
        }
    }

public:
    float x_paddle, y_paddle;
    float width_paddle, height_paddle;
    int speed_paddle;

    void Draw()
    {
        DrawRectangle(x_paddle, y_paddle, width_paddle, height_paddle, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y_paddle -= speed_paddle;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y_paddle += speed_paddle;
        }
        LimitMovement();
    }
};

class CpuPaddle : public Paddle
{
public:
    void Update(int ball_y)
    {
        if (y_paddle + height_paddle > ball_y)
        {
            y_paddle -= speed_paddle;
        }
        if (y_paddle + height_paddle / 2 <= ball_y)
        {
            y_paddle += speed_paddle;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle Player;
CpuPaddle Cpu;

int main()
{

    const int screen_width = 1280;
    const int screen_height = 800;

    InitWindow(screen_width, screen_height, "Pong Game");
    SetTargetFPS(60);
    ball.radius = 20;
    ball.x_ball = screen_width / 2;
    ball.y_ball = screen_height / 2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    Player.width_paddle = 25;
    Player.height_paddle = 120;
    Player.x_paddle = screen_width - Player.width_paddle - 10;
    Player.y_paddle = screen_height / 2 - Player.height_paddle / 2;
    Player.speed_paddle = 6;

    Cpu.height_paddle = 120;
    Cpu.width_paddle = 25;
    Cpu.x_paddle = 10;
    Cpu.y_paddle = screen_height / 2 - Cpu.height_paddle / 2;
    Cpu.speed_paddle = 6;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ball.Update();
        Player.Update();
        Cpu.Update(ball.y_ball);

        if (CheckCollisionCircleRec(Vector2{ball.x_ball, ball.y_ball}, ball.radius, Rectangle{Player.x_paddle, Player.y_paddle, Player.width_paddle, Player.height_paddle}))
        {
            ball.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{ball.x_ball, ball.y_ball}, ball.radius, Rectangle{Cpu.x_paddle, Cpu.y_paddle, Cpu.width_paddle, Cpu.height_paddle}))
        {
            ball.speed_x *= -1;
        }

        ClearBackground(GREEN);
        ball.Draw();

        Player.Draw();

        Cpu.Draw();

        DrawLine(screen_width / 2, 0, screen_width / 2, screen_height, RED);
        DrawText(TextFormat("%i", cpu_score), screen_width / 4, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width / 4, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
