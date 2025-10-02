#pragma once

#include <memory>

namespace scbe {

template<typename T, typename U>
std::shared_ptr<T> cast(std::shared_ptr<U> ptr) {
    return std::static_pointer_cast<T>(ptr);
}

template<typename T, typename U>
std::shared_ptr<T> dyn_cast(std::shared_ptr<U> ptr) {
    return std::dynamic_pointer_cast<T>(ptr);
}

template<typename T, typename U>
T* cast(U* ptr) {
    return static_cast<T*>(ptr);
}

template<typename T, typename U>
T* dyn_cast(U* ptr) {
    return dynamic_cast<T*>(ptr);
}

}