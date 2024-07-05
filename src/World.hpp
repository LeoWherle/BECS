
#pragma once

#include "EntityManager.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

#include "ComponentTable.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "View.hpp"

#ifdef DEBUG
template<class C>
// needs this function :
// friend std::ostream &operator<<(std::ostream &os, const C &obj)
concept ComponentType = requires(C c) {
    { std::cout << c };
};
#else
template<class C>
concept ComponentType = true;
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
    EntityManager entityManager;

public:
    World() = default;

    Entity createEntity(const std::string &name = "unknown") { return entityManager.create(name); }

    void destroyEntity(Entity entity)
    {
        for (auto &[id, table] : tables) {
            table->remove(entity);
        }
        entityManager.destroy(entity);
    }

    size_t getEntityCount() const { return entityManager.getEntities().size(); }

    template<ComponentType Component>
    World &registerComponent()
    {
        const size_t hash = typeid(Component).hash_code();
        tables[hash] = std::make_unique<ComponentTable<Component>>();
#ifdef DEBUG
        names[hash] = typeid(Component).name();
#endif
        return *this;
    }

    template<ComponentType... Components>
    void Entityadd(Entity entity, const Components &...components)
    {
        (EntityaddComponent(entity, components), ...);
    }

    template<ComponentType Component>
    void EntityaddComponent(Entity entity, const Component &component)
    {
        const size_t hash = typeid(Component).hash_code();

        auto it = tables.find(hash);
        if (it == tables.end()) {
            throw std::runtime_error("Component not found");
        } else {
            it->second->add(entity, component);
        }
    }

    template<ComponentType Component>
    Component Entityget(Entity entity)
    {
        auto &table = getTable<Component>();
        return table.get(entity);
    }

    template<ComponentType Component>
    void Entityremove(Entity entity)
    {
        auto it = tables.find(typeid(Component).hash_code());
        if (it == tables.end()) {
            throw std::runtime_error("Component not found");
        }
        it->second->remove(entity);
    }

    template<ComponentType Component>
    ComponentTable<Component> &getTable()
    {
        auto it = tables.find(typeid(Component).hash_code());
        if (it == tables.end()) {
            throw std::runtime_error("Component not found");
        }
        return *static_cast<ComponentTable<Component> *>(it->second.get());
    }

    template<ComponentType... Component>
    View<Component...> getView()
    {
        return View<Component...>(getTable<Component>()...);
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
