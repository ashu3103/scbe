#include "MIR/constant_pool.hpp"

namespace scbe::MIR {

size_t ConstantPool::addEntry(Ref<IR::Constant> entry) {
    m_pool.push_back(entry);
    return m_pool.size() - 1;
}

const Ref<IR::Constant> ConstantPool::getEntry(size_t index) {
    return m_pool.at(index);
}

}