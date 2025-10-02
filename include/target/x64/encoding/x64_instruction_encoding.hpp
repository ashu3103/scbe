#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <initializer_list>

namespace scbe::Target::x64 {

class InstructionEncoding {
public:
    enum OperandType {
        Normal,
        Embedded,
        Symbol,
        None
    };

    InstructionEncoding() = default;

    constexpr InstructionEncoding(
        std::initializer_list<uint8_t> bytes, bool default64Bit = false, OperandType operandType = OperandType::Normal, bool immediate = false, uint8_t immediateSize = 0, std::optional<uint8_t> instructionVariant = std::nullopt)
        : m_numBytes(bytes.size()), m_default64Bit(default64Bit), m_operandType(operandType), m_immediate(immediate), m_immediateSize(immediateSize), m_instructionVariant(instructionVariant) {
        for(size_t i = 0; i < bytes.size(); i++) {
            m_bytes[i] = bytes.begin()[i];
        }
    }

    InstructionEncoding& setRmIdx(uint8_t idx) { m_rmIdx = idx; return *this; }
    InstructionEncoding& setRegIdx(uint8_t idx) { m_regIdx = idx; return *this; }
    InstructionEncoding& setRexPosition(uint8_t idx) { m_rexPosition = idx; return *this; }

public:

    static constexpr size_t s_instructionBytes = 3;
    std::array<uint8_t, s_instructionBytes> m_bytes = {0x00, 0x00, 0x00};
    size_t m_numBytes = 0;
    bool m_default64Bit = false;
    OperandType m_operandType = OperandType::Normal;
    bool m_immediate = false;
    uint8_t m_immediateSize = 0;
    std::optional<uint8_t> m_instructionVariant;

    uint8_t m_rmIdx = 0;
    uint8_t m_regIdx = 1;
    uint8_t m_rexPosition = 0;
};

}