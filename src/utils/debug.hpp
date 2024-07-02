
#pragma once

#include <iostream>
#include <string>
#include <tuple>

namespace dbg {

// Helper to print a tuple (used for multiple member variables)
template<std::size_t Index = 0, typename... Types>
constexpr typename std::enable_if<Index == sizeof...(Types), void>::type
print_tuple(std::ostream &os, const std::tuple<Types...> &)
{
}

template<std::size_t Index = 0, typename... Types>
    constexpr typename std::enable_if <
    Index<sizeof...(Types), void>::type print_tuple(std::ostream &os, const std::tuple<Types...> &t)
{
    if (Index != 0)
        os << ", ";
    os << std::get<Index>(t);
    print_tuple<Index + 1>(os, t);
}

// Template function to generate the operator<< overload
template<typename T, typename... Args>
std::ostream &
print(std::ostream &os, const T &obj, const std::string &className, const std::tuple<Args...> &args)
{
    os << className << "(";
    print_tuple(os, args);
    os << ")";
    return os;
}

} // namespace dbg

// Macro to automatically generate the operator<< overload
#define DERIVE_DEBUG(ClassName, ...)                                        \
    inline auto get_members() const { return std::tie(__VA_ARGS__); }       \
    friend std::ostream &operator<<(std::ostream &os, const ClassName &obj) \
    {                                                                       \
        return dbg::print(os, obj, #ClassName, obj.get_members());          \
    }
