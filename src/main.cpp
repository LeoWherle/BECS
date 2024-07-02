
#include "Entity.hpp"

int main()
{
    Entity e(1);

    Entity e2(2, "test");

    std::cout << e << std::endl;
    std::cout << e2 << std::endl;
  
    std::cout << "size of Entity: " << sizeof(Entity) << "\n"; // Should print "size of Entity: 8"

    return 0;
}
