
#pragma once

#include <any>
#include <cstddef>
#include <memory>

#ifdef DEBUG
#include <iostream>
#endif

#include "ComponentTable.hpp"
#include "Entity.hpp"

#ifdef DEBUG
template<class C>
// needs this function :
// friend std::ostream &operator<<(std::ostream &os, const C &obj)
concept Component = requires(C c) {
    { std::cout << c };
};
#endif

// table for a specific component with the entity id as the key and the component as the value

// class registering components that can be attached to an entity
// each component has a unique id
// each component has a unique table with the entity id as the key and the component as the value
class World {
private:
    std::unordered_map<size_t, std::unique_ptr<IComponentTable>> tables;
#ifdef DEBUG
    std::unordered_map<size_t, std::string> names;
#endif

public:
    World() = default;

    template<typename Component>
    void add(Entity entity, const Component &component)
    {
        const size_t hash = typeid(Component).hash_code();

        auto it = tables.find(hash);
        if (it == tables.end()) {
            auto table = std::make_unique<ComponentTable<Component>>();
            table->add(entity, component);
            tables[hash] = std::move(table);
#ifdef DEBUG
            names[hash] = typeid(Component).name();
#endif
        } else {
            it->second->add(entity, component);
        }
    }

    template<typename Component>
    Component get(Entity entity)
    {
        const size_t hash = typeid(Component).hash_code();

        auto it = tables.find(hash);
        if (it == tables.end()) {
            throw std::runtime_error("Component not found");
        }
        return std::any_cast<Component>(it->second->get(entity));
    }

    template<typename Component>
    void remove(Entity entity)
    {
        auto it = tables.find(typeid(Component).hash_code());
        if (it == tables.end()) {
            throw std::runtime_error("Component not found");
        }
        it->second->remove(entity);
    }

    template<typename Component>
    ComponentTable<Component> &getTable()
    {
        auto it = tables.find(typeid(Component).hash_code());
        if (it == tables.end()) {
            throw std::runtime_error("Component not found");
        }
        return *static_cast<ComponentTable<Component> *>(it->second.get());
    }

#ifdef DEBUG
    friend std::ostream &operator<<(std::ostream &os, const World &cr)
    {
        os << "{\n";
        for (const auto &[id, table] : cr.tables) {
            auto val = cr.names.find(id);
            if (val == cr.names.end()) {
                throw std::runtime_error("Component name not found");
            }
            const std::string &name = val->second;
            os << name << "Table : " << *table << "\n";
        }
        os << "}";
        return os;
    }
#endif
};
