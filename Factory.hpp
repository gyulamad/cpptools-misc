#pragma once

#include <utility>
#include <type_traits>
#include <unordered_map>
#include <functional>

#include "array_key_exists.hpp"
#include "ERROR.hpp"

using namespace std;

class Factory {
protected:
    unordered_map<void*, function<void()>> instances; 
public:
    Factory() {}
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

    virtual ~Factory() {
        destroyAll();
    }
    
    template<typename T, typename ...Args>
    T* create(Args&&... args) {
        T* instance = new T(forward<Args>(args)...);
        return store(instance);
    }

    template<typename T>
    T* store(T* instance) {
        static_assert(has_virtual_destructor_v<T>, "No virtual destructor");
        if (array_key_exists(instance, instances)) return instance;
        instances[instance] = [instance]() {
            delete instance;
        };
        return instance;
    }

    template<typename TBase, typename TDerived>
    TBase* storeAs(TBase* instance) {
        if (array_key_exists(instance, instances)) return instance;
        instances[instance] = [instance]() { 
            delete static_cast<TDerived*>(instance); 
        };
        return instance;
    }

    template<typename T>
    bool destroy(T* instance, bool throws = true) {
        auto it = instances.find(instance);
        if (it != instances.end()) {
            auto destroyer = std::move(it->second);
            instances.erase(it);
            destroyer();
            return true;
        }
        if (throws) throw ERROR("Instance is not found");
        return false;
    }

    void destroyAll() {
        for (auto& instance: instances) instance.second();
        instances.clear();
    }
    
};

