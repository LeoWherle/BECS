
#include "Entity.hpp"
#include "World.hpp"
#include "utils/debug.hpp"

struct Position {
    float x, y;

    DERIVE_DEBUG(Position, x, y)
};

struct Velocity {
    float x, y;

    DERIVE_DEBUG(Velocity, x, y)
};

int main()
{
    Entity e(1);

    Entity e2(2, "test");

    std::cout << e << std::endl;
    std::cout << e2 << std::endl;

    std::cout << "size of Entity: " << sizeof(Entity) << "\n"; // Should print "size of Entity: 8"

    World world;

    world.add<Position>(e, {1.0f, 2.0f});
    world.add<Position>(e2, {1.0f, 2.0f});
    world.add(e2, Velocity {3.0f, 4.0f});

    auto pos = world.get<Position>(e);

// std::cout << pos << std::endl;
#ifdef DEBUG
    std::cout << "\n" << world << std::endl;
#endif
    return 0;
}
