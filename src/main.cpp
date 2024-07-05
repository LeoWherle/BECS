
#include <chrono>
#include <iostream>
#include <thread>

#include "raylib.h"

#include "Entity.hpp"
#include "World.hpp"
#include "utils/debug.hpp"

struct Position {
    float x, y;

    DERIVE_DEBUG(Position, x, y)
};

struct Velocity {
    float vx, vy;

    DERIVE_DEBUG(Velocity, vx, vy)
};

struct Size {
    float radius;

    DERIVE_DEBUG(Size, radius)
};

struct BallColor {
    union {
        struct {
            uint8_t r, g, b, a;
        };
        uint32_t value;
    };

    DERIVE_DEBUG(BallColor, r, g, b, a)
};

class MovementSystem {
public:
    void update(World &world, float deltaTime)
    {
        auto view = world.getView<Position, Velocity>();
        view.each([deltaTime](Entity entity, Position &pos, Velocity &vel) {
            pos.x += vel.vx * deltaTime;
            pos.y += vel.vy * deltaTime;
        });
    }
};

class CollisionSystem {
private:
    float minX, minY, maxX, maxY;

public:
    CollisionSystem(float minX, float minY, float maxX, float maxY):
        minX(minX),
        minY(minY),
        maxX(maxX),
        maxY(maxY)

    {
    }

    void update(World &world)
    {
        auto view = world.getView<Position, Velocity, Size>();
        view.each([this](Entity entity, Position &pos, Velocity &vel, Size &size) {
            if (pos.x - size.radius <= minX || pos.x + size.radius >= maxX) {
                vel.vx = -vel.vx;
                pos.x = std::clamp(pos.x, minX + size.radius, maxX - size.radius);
            }
            if (pos.y - size.radius <= minY || pos.y + size.radius >= maxY) {
                vel.vy = -vel.vy;
                pos.y = std::clamp(pos.y, minY + size.radius, maxY - size.radius);
            }
        });
    }
};

class RenderSystem {
public:
    void render(World &world)
    {
        auto view = world.getView<Position, Size, BallColor>();
        view.each([](Entity entity, Position &pos, Size &size, BallColor &color) {
            DrawCircle(pos.x, pos.y, size.radius, Color {color.r, color.g, color.b, color.a});
        });
    }
};

float GetRandomFloat(int min, int max)
{
    return static_cast<float>(GetRandomValue(min, max));
}

uint8_t GetRandomChar(int min, int max)
{
    return static_cast<uint8_t>(GetRandomValue(min, max));
}

void generateBalls(World &world, int numBalls)
{
    for (int i = 0; i < numBalls; i++) {
        auto ball = world.createEntity("ball" + std::to_string(i));
        world.Entityadd(
            ball, Position {GetRandomFloat(0, 800), GetRandomFloat(0, 600)},
            Velocity {GetRandomFloat(-200, 200), GetRandomFloat(-200, 200)}, Size {GetRandomFloat(2, 3)},
            BallColor {GetRandomChar(0, 255), GetRandomChar(0, 255), GetRandomChar(0, 255), 255}
        );
    }
}

int main()
{
    InitWindow(800, 600, "Bouncing Balls Simulation");
    SetTargetFPS(60);

    World world;

    world.registerComponent<Position>()
        .registerComponent<Velocity>()
        .registerComponent<Size>()
        .registerComponent<BallColor>();

    auto ball_red = world.createEntity("ballRed");
    world.Entityadd(
        ball_red, Position {50.0f, 50.0f}, Velocity {100.0f, 150.0f}, Size {20.0f}, BallColor {255, 0, 0, 255}
    );
    auto ball_green = world.createEntity("ballGreen");
    world.Entityadd(
        ball_green, Position {100.0f, 200.0f}, Velocity {-200.0f, 100.0f}, Size {30.0f},
        BallColor {0, 255, 0, 255}
    );

    generateBalls(world, 10000);

    MovementSystem movementSystem;
    CollisionSystem collisionSystem(0.0f, 0.0f, 800.0f, 600.0f);
    RenderSystem renderSystem;

    while (!WindowShouldClose()) {
        // Update systems
        float deltaTime = GetFrameTime();
        movementSystem.update(world, deltaTime);
        collisionSystem.update(world);

        // Update camera computes movement internally depending on the camera mode
        // Some default standard keyboard/mouse inputs are hardcoded to simplify use
        // For advance camera controls, it's reecommended to compute camera movement manually
        // Draw
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            renderSystem.render(world);

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();

// std::cout << pos << std::endl;
#ifdef DEBUG
    std::cout << "\n" << world << std::endl;
    std::cout << "Number of entities: " << world.getEntityCount() << std::endl;
#endif
    return 0;
}
