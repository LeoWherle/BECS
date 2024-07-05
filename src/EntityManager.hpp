
#pragma once

#include <atomic>
#include <string>
#include <vector>
#include <algorithm>

#include "Entity.hpp"

// creates, destroys, and manages entities
class EntityManager {
private:
    std::vector<Entity> entities;

public:
    EntityManager() = default;

    Entity create(const std::string &name = "unknown")
    {
        static std::atomic<std::size_t> id = 0;
        entities.emplace_back(id);
        return Entity(id++, name);
    }

    void destroy(Entity entity)
    {
        entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
    }

    const std::vector<Entity> &getEntities() const { return entities; }

    void clear() { entities.clear(); }

    size_t size() const { return entities.size(); }
};
