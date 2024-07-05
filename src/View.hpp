#pragma once

#include "ComponentTable.hpp"

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
