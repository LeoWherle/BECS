#pragma once

#include <iostream>
#include <string>

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

    operator std::size_t() const { return id; }

    operator std::string() const
    {
#ifdef DEBUG
        return name;
#else
        return "unknown";
#endif
    }

    friend std::ostream &operator<<(std::ostream &os, const Entity &entity)
    {
#ifdef DEBUG
        os << "Entity(" << entity.id << ", " << entity.name << ")";
#else
        os << "Entity(" << entity.id << ")";
#endif
        return os;
    }

private:
    std::size_t id;
#ifdef DEBUG
    std::string name;
#endif
};
