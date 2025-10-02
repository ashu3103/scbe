#pragma once

#include "type_alias.hpp"

namespace scbe {

class Type;

class DataLayout {
public:
    virtual size_t getPointerSize() const = 0;
    virtual size_t getSize(Type* type) const = 0;
    virtual size_t getAlignment(Type* type) const = 0;
};

}