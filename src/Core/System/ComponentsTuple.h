#pragma once

#include "../Entity/Entity.h"

template<class... Ts>
class ComponentsTuple {
public:
    typedef std::tuple<Ts *...> TTuple;

    template<class T>
    T *getValue() {
        typedef T *TP;

        return std::get<TP>(m_tuple);
    }

    template<class T>
    void setValue(T *p) {
        typedef T *TP;

        std::get<TP>(m_tuple) = p;
    }

    void registerComponents(Entity &entity) {
        if (!doesEntityContainAllComponents(entity)) {
            return;
        }

        _registerComponents(entity);
    }

    static bool doesEntityContainAllComponents(Entity &e) {
        return _doesEntityContainAllComponents(e);
    }

    template<std::size_t I = 0>
    void debugPrint() {
        auto cp = std::get<I>(m_tuple);

        typedef typename std::remove_pointer<std::tuple_element_t<I, TTuple>>::type ComponentT;

        Log::write(ComponentT::TypeName(), " => ", cp);

        if constexpr (I + 1 != sizeof...(Ts)) {
            debugPrint<I + 1>();
        }
    }

    const TTuple &get() const {
        return m_tuple;
    }

private:
    template<std::size_t I = 0>
    static bool _doesEntityContainAllComponents(Entity &e) {
        typedef typename std::remove_pointer<std::tuple_element_t<I, TTuple>>::type ComponentT;

        if (!e.hasComponent<ComponentT>()) {
            return false;
        }

        if constexpr (I + 1 != sizeof...(Ts)) {
            return _doesEntityContainAllComponents<I + 1>(e);
        }

        return true;
    }

    template<std::size_t I = 0>
    void _registerComponents(Entity &entity) {
        typedef typename std::remove_pointer<std::tuple_element_t<I, TTuple>>::type ComponentT;

        std::get<I>(m_tuple) = entity.getComponent<ComponentT>();

        if constexpr (I + 1 != sizeof...(Ts)) {
            _registerComponents<I + 1>(entity);
        }
    }

    TTuple m_tuple;
};