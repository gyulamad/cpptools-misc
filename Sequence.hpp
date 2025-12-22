#pragma once

#include <vector>
#include "array_unique.hpp"
#include "ERROR.hpp"

using namespace std;

template<typename T>
class Sequence: public vector<T> {
public:
    using vector<T>::vector;
    
    // Move constructor
    Sequence(Sequence&& other) noexcept 
        : vector<T>(std::move(other)), 
          mode_ordered(other.mode_ordered),
          orders(std::move(other.orders)) {}
    
    // Move assignment
    Sequence& operator=(Sequence&& other) noexcept {
        vector<T>::operator=(std::move(other));
        mode_ordered = other.mode_ordered;
        orders = std::move(other.orders);
        return *this;
    }

    // Copy constructor
    Sequence(const Sequence& other) 
        : vector<T>(other),
          mode_ordered(other.mode_ordered),
          orders(other.orders) {}

    // Copy assignment
    Sequence& operator=(const Sequence& other) {
        vector<T>::operator=(other);
        mode_ordered = other.mode_ordered;
        orders = other.orders;
        return *this;
    }

    // Keep your existing move operations
    // Sequence(Sequence&& other) noexcept = default;
    // Sequence& operator=(Sequence&& other) noexcept = default;

    virtual ~Sequence() = default;

    void reconstruct(vector<T> elements, const vector<size_t>& orders = {}, bool strict = false) {
        this->orders = array_unique(orders);
        if (strict && elements.size() != this->orders.size())
            throw ERROR("Elements and orders size mismatch");
        this->clear();
        for (const T& element: elements) this->push_back(element);
        fix_orders();
    }

    void set_mode_ordered(bool mode_ordered) { this->mode_ordered = mode_ordered; }

    const vector<size_t>& get_orders_cref() { 
        fix_orders();
        return orders;
    }

    // vector<T>& get_elements_cref() { return *this; }

    void insert_at(size_t at, const T& element) {
        fix_orders();
        this->push_back(element);
        size_t last = this->size() - 1;
        orders.push_back(last);
        replace_with(last, at);
    }

    void move_to(size_t at, size_t to) {
        if (at < to) move_backward(at, to - at);
        if (at > to) move_forward(at, at - to);
    }

    void replace_with(size_t at, size_t to) {
        fix_orders();
        if (at >= orders.size() || to >= orders.size())
            throw ERROR("Order position is out of bounds or invalid");
        if (at == to) return;
        size_t tmp = orders[at]; // TODO: is there a way to swap two elements in an unordered set??
        orders[at] = orders[to];
        orders[to] = tmp;
    }

    void move_backward(size_t at, size_t d = 1) {
        fix_orders();
        if (at + 1 >= orders.size()) return;
        for (size_t i = 0; i < d; i++)
            replace_with(at + i, at + i + 1);
    }

    void move_forward(size_t at, size_t d = 1) {
        if (at == 0) return;
        for (size_t i = 0; i < d; i++)
            replace_with(at - i, at - i - 1);
    }

    //=== Mode-Specific Accessors ===//
    T& operator[](size_t at) {
        fix_orders();
        return vector<T>::operator[](mode_ordered ? orders[at] : at);
    }

    const T& operator[](size_t at) const {
        return vector<T>::operator[](mode_ordered ? orders[at] : at);
    }

    T& at(size_t at) {
        fix_orders();
        return vector<T>::at(mode_ordered ? orders[at] : at);
    }

    const T& at(size_t at) const {
        return vector<T>::at(mode_ordered ? orders[at] : at);
    }

protected:
    bool mode_ordered = false;
    vector<size_t> orders = {};

    void fix_orders() {
        if (orders.size() > this->size()) orders = array_slice(orders, 0, this->size());
        for (size_t n = orders.size(); n < this->size(); n++) orders.push_back(n);
    }
};

