#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>

template<typename Component, typename Allocator = std::allocator<Component>>
class SparseArray {
public:
    using value_type = Component;
    using reference_type = value_type &;
    using const_reference_type = const value_type &;
    using container_t = std::vector<value_type, Allocator>;
    using size_type = typename container_t::size_type;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

public:
    SparseArray() = default; 
    SparseArray(const SparseArray &other) = default; // copy constructor
    SparseArray(SparseArray &&other) noexcept = default; // move constructor
    ~SparseArray() = default;
    SparseArray &operator=(const SparseArray &other) = default; // copy assignment operator
    SparseArray &operator=(SparseArray &&other) noexcept = default; // move assignment operator

    reference_type operator[](size_t idx) {
        if (idx >= _data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return _data[idx];
    }

    const_reference_type operator[](size_t idx) const {
        if (idx >= _data.size()) {
            throw std::out_of_range("Index out of range");
        }
        return _data[idx];
    }

    iterator begin() {
        return _data.begin();
    }

    const_iterator begin() const {
        return _data.begin();
    }

    const_iterator cbegin() const {
        return _data.cbegin();
    }

    iterator end() {
        return _data.end();
    }

    const_iterator end() const {
        return _data.end();
    }

    const_iterator cend() const {
        return _data.cend();
    }

    size_type size() const {
        return _data.size();
    }

    reference_type insert_at(size_type pos, const Component &value) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = value;
        return _data[pos];
    }

    reference_type insert_at(size_type pos, Component &&value) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = std::move(value);
        return _data[pos];
    }

    template<class... Params>
    reference_type emplace_at(size_type pos, Params &&... params) {
        if (pos >= _data.size()) {
            _data.resize(pos + 1);
        }
        _data[pos] = value_type(std::forward<Params>(params)...);
        return _data[pos];
    }

    void erase(size_type pos) {
        if (pos < _data.size()) {
            _data.erase(_data.begin() + pos);
        } else {
            throw std::out_of_range("Index out of range");
        }
    }

    size_type get_index(const value_type &value) const {
        auto it = std::find(_data.begin(), _data.end(), value);
        if (it != _data.end()) {
            return std::distance(_data.begin(), it);
        }
        throw std::out_of_range("Value not found in array");
    }

private:
    container_t _data;
};
