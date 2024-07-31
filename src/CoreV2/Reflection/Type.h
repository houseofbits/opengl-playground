#pragma once

template<class T, class T2>
bool isOfType(T2& p) {
    return dynamic_cast<T*>(p);
}