#include "components/CVelocity.hpp"
#include "raylib.h"

#include "Entity.hpp"
#include "World.hpp"

#include "components/components.hpp"
#include "systems/systems.hpp"


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
            ball, CPosition {GetRandomFloat(0, 800), GetRandomFloat(0, 600)},
            CVelocity {GetRandomFloat(-200, 200), GetRandomFloat(-200, 200)}, CCircle {GetRandomFloat(2, 3)},
            CShapeColor {GetRandomChar(0, 255), GetRandomChar(0, 255), GetRandomChar(0, 255), 255}
        );
    }
}

int main()
{
    InitWindow(800, 600, "Bouncing Balls Simulation");
    SetTargetFPS(60);

    World world;

    world.registerComponent<CPosition>()
        .registerComponent<CVelocity>()
        .registerComponent<CCircle>()
        .registerComponent<CShapeColor>()
        .registerComponent<CRectangle>();

    auto ball_red = world.createEntity("ballRed");
    world.Entityadd(
        ball_red, CPosition {50.0f, 50.0f}, CVelocity {100.0f, 150.0f}, CCircle {20.0f}, CShapeColor {255, 0, 0, 255}
    );
    auto ball_green = world.createEntity("ballGreen");
    world.Entityadd(
        ball_green, CPosition {100.0f, 200.0f}, CVelocity {-200.0f, 100.0f}, CCircle {30.0f},
        CShapeColor {0, 255, 0, 255}
    );
    auto rec_blue = world.createEntity("recBlue");
    world.Entityadd(
        rec_blue, CPosition {200.0f, 300.0f}, CRectangle {40.0f, 60.0f}, CVelocity {50.0f, 50.0f},
        CShapeColor {0, 0, 255, 255}
    );
    // generateBalls(world, 10000);

    SMovement movementSystem;
    SCollision collisionSystem(0.0f, 0.0f, 800.0f, 600.0f);
    SRenderCircle renderSystem;
    SRenderRectangle renderRectangleSystem;

    while (!WindowShouldClose()) {
        // Update systems
        float deltaTime = GetFrameTime();
        movementSystem.update(world, deltaTime);
        collisionSystem.update(world);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            renderSystem.render(world);
            renderRectangleSystem.render(world);

            DrawFPS(10, 10);
        }
        EndDrawing();
    }

    CloseWindow();

#ifdef DEBUG
    std::cout << "\n" << world << std::endl;
    std::cout << "Number of entities: " << world.getEntityCount() << std::endl;
#endif
    return 0;
}
