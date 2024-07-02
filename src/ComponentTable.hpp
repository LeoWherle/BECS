
#pragma once

#include "Entity.hpp"
#include <any>
#include <cstddef>
#include <iostream>
#include <ostream>
#include <unordered_map>

class IComponentTable {
public:
    virtual ~IComponentTable() = default;

    virtual void add(Entity entity, const std::any &component) = 0;
    virtual std::any get(Entity entity) = 0;
    virtual void remove(Entity entity) = 0;

    friend std::ostream &operator<<(std::ostream &os, const IComponentTable &table)
    {
        table.print(os);
        return os;
    }

private:
    virtual void print(std::ostream &os) const = 0;
};

template<typename Component>
class ComponentTable : public IComponentTable {
private:
    std::unordered_map<Entity, Component> table;

public:
    using iterator = typename std::unordered_map<Entity, Component>::iterator;
    using const_iterator = typename std::unordered_map<Entity, Component>::const_iterator;

public:
    ComponentTable() = default;
    ~ComponentTable() override = default;
    ComponentTable(const ComponentTable &other) = default;
    ComponentTable(ComponentTable &&other) noexcept = default;
    ComponentTable &operator=(const ComponentTable &other) = default;
    ComponentTable &operator=(ComponentTable &&other) noexcept = default;


    void add(Entity entity, const std::any &component) override
    {
        table[entity] = std::any_cast<Component>(component);
    }
    std::any get(Entity entity) override { return table[entity]; }
    void remove(Entity entity) override { table.erase(entity); }
    iterator begin() { return table.begin(); }
    iterator end() { return table.end(); }

    void print(std::ostream &os) const override
    {
        os << "{\n";
        for (const auto &[entity, component] : table) {
            os << entity << ": " << component << "\n";
        }
        os << "}";
    }

    friend std::ostream &operator<<(std::ostream &os, const ComponentTable &table)
    {
        table.print(os);
        return os;
    }
};
