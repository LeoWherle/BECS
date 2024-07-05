
#pragma once

#include <tuple>
#include <vector>

// Table<VALUE, VALUE, VALUE, ...>
// Table using struct of arrays principle instead of array of structs
// table <T> {
//  array<t1> col1;
//  array<t2> col2;
//}
template<typename... Args>
struct TupleOfVectors {
    using type = std::tuple<std::vector<Args>...>;
};

template<typename... Args>
using TupleOfVectors_t = typename TupleOfVectors<Args...>::type;

// Main Table class
template<typename... Args>
class Table {
private:
    TupleOfVectors_t<Args...> columns;

    template<size_t... Is>
    auto get_helper(size_t row, std::index_sequence<Is...>)
    {
        return std::make_tuple(std::get<Is>(columns)[row]...);
    }

public:
    // Add a row to the table
    void add(Args... args)
    {
        std::apply(
            [&](auto &...vecs) {
                (vecs.push_back(args), ...);
            },
            columns
        );
    }

    // Get a specific row with specific columns
    template<size_t... Is>
    auto get(size_t row)
    {
        return get_helper(row, std::index_sequence<Is...> {});
    }
};

#ifdef TABLE_MAIN
int main()
{
    Table<int, std::string, int, int> myTable;

    // Add some data to the table
    myTable.add(0, "test", 12, 3);
    myTable.add(1, "test2", 13, 4);

    // Retrieve data from the table
    auto val = myTable.get<0, 2>(0); // Get the first and third columns of the first row

    // Check if the retrieved data is as expected
    assert(val == std::make_tuple(0, 12));

    std::cout << "Value: " << std::get<0>(val) << ", " << std::get<1>(val)
              << "\n"; // Should print "Value: 0, 12
    std::cout << "Table:\n";
    for (size_t i = 0; i < 2; ++i) {
        auto val = myTable.get<0, 1, 2, 3>(i);
        std::cout << std::get<0>(val) << ", " << std::get<1>(val) << ", " << std::get<2>(val) << ", "
                  << std::get<3>(val) << "\n";
    }
    return 0;
}
#endif