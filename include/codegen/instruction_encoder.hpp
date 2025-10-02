#pragma once

#include "codegen/fixup.hpp"
#include "MIR/instruction.hpp"
#include "target/instruction_info.hpp"

#include <cstdint>
#include <optional>
#include <vector>

namespace scbe::Codegen {

class InstructionEncoder {
public:
    InstructionEncoder(Target::InstructionInfo* info) : m_instructionInfo(info), m_registerInfo(info->getRegisterInfo()) {}

    virtual std::optional<Fixup> encode(MIR::Instruction* instruction, UMap<std::string, size_t> symbols, std::vector<uint8_t>& bytes) = 0;

protected:
    Target::InstructionInfo* m_instructionInfo = nullptr;
    Target::RegisterInfo* m_registerInfo = nullptr;
};

}