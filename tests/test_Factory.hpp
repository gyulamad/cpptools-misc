#pragma once

#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../Factory.hpp"

class VirtualDtorClass {
public:
    virtual ~VirtualDtorClass() {}
    int value = 0;
};

class BaseClass {
public:
    virtual ~BaseClass() {}
    int base_value = 0;
};

class DerivedClass : public BaseClass {
public:
    int derived_value = 0;
};

TEST(test_Factory_create_and_auto_destroy) {
    Factory factory;
    VirtualDtorClass* obj = factory.create<VirtualDtorClass>();
    obj->value = 42;
    assert(obj != nullptr && "create should return non-null pointer");
}

TEST(test_Factory_create_with_constructor_args) {
    Factory factory;
    VirtualDtorClass* obj = factory.create<VirtualDtorClass>();
    obj->value = 100;
    assert(obj->value == 100 && "create should allow setting values after construction");
}

TEST(test_Factory_store_and_destroy) {
    Factory factory;
    VirtualDtorClass* obj = new VirtualDtorClass();
    obj->value = 55;
    VirtualDtorClass* stored = factory.store(obj);
    assert(stored == obj && "store should return the same pointer");
    bool destroyed = factory.destroy<VirtualDtorClass>(obj, false);
    assert(destroyed && "destroy should return true for registered instance");
}

TEST(test_Factory_store_duplicate_returns_existing) {
    Factory factory;
    VirtualDtorClass* obj = new VirtualDtorClass();
    factory.store(obj);
    VirtualDtorClass* again = factory.store(obj);
    assert(again == obj && "store should return same pointer on duplicate");
}

TEST(test_Factory_storeAs_polymorphic_delete) {
    Factory factory;
    DerivedClass* derived = new DerivedClass();
    derived->base_value = 10;
    derived->derived_value = 20;
    BaseClass* stored = factory.storeAs<BaseClass, DerivedClass>(derived);
    assert(stored == derived && "storeAs should return the same pointer");
}

TEST(test_Factory_destroy_not_found_no_throw) {
    Factory factory;
    VirtualDtorClass* obj = new VirtualDtorClass();
    bool destroyed = factory.destroy<VirtualDtorClass>(obj, false);
    assert(!destroyed && "destroy should return false for unregistered instance");
    delete obj;
}

TEST(test_Factory_destroy_not_found_throws) {
    Factory factory;
    VirtualDtorClass* obj = new VirtualDtorClass();
    bool threw = false;
    try {
        factory.destroy<VirtualDtorClass>(obj, true);
    } catch (exception& e) {
        assert(str_contains(e.what(), "Instance is not found") && "Exception should contain expected message");
        threw = true;
    }
    assert(threw && "destroy should throw for unregistered instance when throws=true");
    delete obj;
}

TEST(test_Factory_destroyAll) {
    Factory factory;
    VirtualDtorClass* obj1 = factory.create<VirtualDtorClass>();
    VirtualDtorClass* obj2 = factory.create<VirtualDtorClass>();
    obj1->value = 1;
    obj2->value = 2;
}

TEST(test_Factory_multiple_creates_and_destroys) {
    Factory factory;
    VirtualDtorClass* obj1 = factory.create<VirtualDtorClass>();
    VirtualDtorClass* obj2 = factory.create<VirtualDtorClass>();
    VirtualDtorClass* obj3 = factory.create<VirtualDtorClass>();
    obj1->value = 10;
    obj2->value = 20;
    obj3->value = 30;
    assert(factory.destroy<VirtualDtorClass>(obj2, false) && "Should destroy middle object");
    assert(!factory.destroy<VirtualDtorClass>(obj2, false) && "Second destroy should return false");
}

TEST(test_Factory_destroy_order_independent) {
    Factory factory;
    VirtualDtorClass* obj1 = factory.create<VirtualDtorClass>();
    VirtualDtorClass* obj2 = factory.create<VirtualDtorClass>();
    VirtualDtorClass* obj3 = factory.create<VirtualDtorClass>();
    assert(factory.destroy<VirtualDtorClass>(obj3, false) && "Should destroy last created");
    assert(factory.destroy<VirtualDtorClass>(obj1, false) && "Should destroy first created");
    assert(factory.destroy<VirtualDtorClass>(obj2, false) && "Should destroy remaining object");
}

#endif