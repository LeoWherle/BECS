
#pragma once

#include "EntityManager.hpp"
#include <algorithm>
#include <any>
#include <atomic>
#include <cstddef>
#include <memory>
#include <thread>
#include <unordered_map>
#include <vector>

#ifdef DEBUG
#include <iostream>
#endif

#include "ComponentTable.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
#include "utils/ThreadPool.hpp"

#ifdef DEBUG
template<class C>
// needs this function :
// friend std::ostream &operator<<(std::ostream &os, const C &obj)
concept Component = requires(C c) {
    { std::cout << c };
};
#endif

// class containing a reference to N componentTables, and makes it easy to iterate over them
template<typename... Components>
class View {
private:
    std::tuple<ComponentTable<Components> &...> tables;

    template<std::size_t... Is>
    auto
    get_components_for_entity(Entity entity, std::index_sequence<Is...>) const -> std::tuple<Components &...>
    {
        return std::tie(std::get<Is>(tables).get(entity)...);
    }

    template<std::size_t... Is>
    bool entity_has_all_components(Entity entity, std::index_sequence<Is...>) const noexcept
    {
        return (std::get<Is>(tables).has(entity) && ...);
    }

public:
    View(ComponentTable<Components> &...tables):
        tables(tables...)
    {
    }

    // Should be used as follows:
    // auto view = world.getView<Component1, Component2, ...>();
    // view.each([](Entity entity, Component1 &c1, Component2 &c2, ...) {
    //     // do something with c1, c2, ...
    // });
    template<typename Func>
    void each(Func func)
    {
        auto &first_table = std::get<0>(tables);
        for (auto [entity, _] : first_table) {
            if (entity_has_all_components(entity, std::index_sequence_for<Components...> {})) {
                auto components =
                    get_components_for_entity(entity, std::index_sequence_for<Components...> {});
                std::apply(
                    [&](auto &...comps) {
                        func(entity, comps...);
                    },
                    components
                );
            }
        }
    }
};

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

    template<class Component>
    World &registerComponent()
    {
        const size_t hash = typeid(Component).hash_code();
        tables[hash] = std::make_unique<ComponentTable<Component>>();
#ifdef DEBUG
        names[hash] = typeid(Component).name();
#endif
        return *this;
    }

    template<typename... Components>
    void Entityadd(Entity entity, const Components &...components)
    {
        (EntityaddComponent(entity, components), ...);
    }

    template<typename Component>
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

    template<typename Component>
    Component Entityget(Entity entity)
    {
        auto &table = getTable<Component>();
        return table.get(entity);
    }

    template<typename Component>
    void Entityremove(Entity entity)
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

    template<typename... Component>
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
