#pragma once

#include <iostream>
#include <string>

#include "utils/debug.hpp"

class Entity {
public:
    explicit Entity(std::size_t id, const std::string &name = "unknown"):
        id(id)
#ifdef DEBUG
        ,
        name(name)
#endif
    {
    }

    bool operator==(const Entity &other) const { return id == other.id; }

    operator std::size_t() const { return id; }
    [[nodiscard]]
    std::size_t getId() const
    {
        return id;
    }

#ifdef DEBUG
    operator std::string() const { return name; }
    DERIVE_DEBUG(Entity, id, name)
#endif

private:
    std::size_t id;
#ifdef DEBUG
    std::string name;
#endif
};

namespace std {
template<>
struct hash<Entity> {
    std::size_t operator()(const Entity &entity) const noexcept
    {
        return std::hash<std::size_t> {}(entity.getId());
    }
};
}
