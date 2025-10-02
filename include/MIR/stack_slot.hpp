#pragma once

#include <cstdint>
namespace scbe::MIR {

struct StackSlot {
    uint32_t m_size;
    int64_t m_offset;
    uint32_t m_alignment;
};

}