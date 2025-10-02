#pragma once

#include "target/x64/encoding/x64_instruction_encoding.hpp"
#include "target/x64/x64_instruction_info.hpp"
#include "target/x64/x64_register_info.hpp"

#include <array>
#include <cstdint>

namespace scbe::Target::x64::Mapping {

extern const std::array<uint8_t, RegisterId::Count> s_registerMapping;
extern const std::array<InstructionEncoding, (size_t)Opcode::Count> s_instructionMapping;

}