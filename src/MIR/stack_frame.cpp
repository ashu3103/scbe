#include "MIR/stack_frame.hpp"
#include "MIR/stack_slot.hpp"

namespace scbe::MIR {

StackSlot StackFrame::addStackSlot(uint32_t size, uint32_t alignment) {
    StackSlot slot{size, 0, alignment};
    m_slots.push_back(slot);
    m_slots.back().m_offset = getSize();
    return slot;
}

size_t StackFrame::getSize() {
    size_t size = 0;
    for(auto slot : m_slots) {
        size += slot.m_alignment > 0 ? size % slot.m_alignment : 0;
        size += slot.m_size;
    }
    return size;
}

}

