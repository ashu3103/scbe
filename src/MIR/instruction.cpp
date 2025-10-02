#include "MIR/instruction.hpp"

namespace scbe::MIR {

std::vector<std::pair<MIR::ImmediateInt*, MIR::Block*>> SwitchLowering::getCases() const {
    std::vector<std::pair<MIR::ImmediateInt*, MIR::Block*>> cases;
    for(int i = 2; i < m_operands.size(); i += 2) {
        cases.emplace_back(cast<MIR::ImmediateInt>(m_operands[i]), cast<MIR::Block>(m_operands[i + 1]));
    }
    return cases;
}
    
}