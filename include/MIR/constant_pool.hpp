#pragma once

#include "IR/value.hpp"
#include "type_alias.hpp"
#include <vector>

namespace scbe::MIR {

class ConstantPool {
public:
    size_t addEntry(Ref<IR::Constant> entry);
    const Ref<IR::Constant> getEntry(size_t index);
    size_t size() { return m_pool.size(); }

private:
    std::vector<Ref<IR::Constant>> m_pool;
    UMap<size_t, size_t> m_cache;
};

}