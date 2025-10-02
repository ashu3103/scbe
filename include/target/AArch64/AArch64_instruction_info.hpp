#pragma once

#include "MIR/block.hpp"
#include "MIR/operand.hpp"
#include "target/instruction_info.hpp"

#include <cstdint>
#include <initializer_list>

namespace scbe::Target::AArch64 {

enum class Opcode : uint32_t {
    Orr64rr, Orr64ri, Orr32rr, Orr32ri,
    Movk64ri, Movk32ri,
    Movz64ri, Movz32ri,
    Movn64ri, Movn32ri,

    Add64rr, Add32rr,
    Add64ri, Add32ri,
    Sub64rr, Sub32rr,
    Sub64ri, Sub32ri,
    Mul64rr, Mul32rr,

    Load64rm, Load32rm,
    Store64rm, Store32rm,

    LoadP64rm, LoadP32rm,
    StoreP64rm, StoreP32rm,

    Ret,

    B, Br, Beq, Bne, Blt, Ble, Bgt, Bge, Bhi, Bls, Bcs, Bcc,

    Subs64rr, Subs32rr,
    Subs64ri, Subs32ri,

    Adrp,

    Fmov64rr, Fmov32rr,
    
    Fadd64rr, Fadd32rr,
    Fsub64rr, Fsub32rr,
    Fmul64rr, Fmul32rr,
    Fdiv64rr, Fdiv32rr,

    Call, Call64r,

    And64rr, And64ri,
    And32rr, And32ri,
    
    Sbfm64, Sbfm32, Ubfm64, Ubfm32,

    Fcvt,
    Fcvtzs64, Fcvtzs32,
    Fcvtzu64, Fcvtzu32,
    Scvtf64, Scvtf32,
    Ucvtf64, Ucvtf32,

    Lslv64, Lslv32,
    Lsrv64, Lsrv32,
    Asrv64, Asrv32,

    Fcmp64rr, Fcmp32rr,

    Csinc64, Csinc32,

    Sdiv64rr, Sdiv32rr,
    Udiv64rr, Udiv32rr,

    Msub64, Msub32,
    
    Count
};

enum class Indexing : uint32_t {
    None,
    PreIndexed,
    PostIndexed
};

enum AArch64OperandFlags : int64_t {
    ExprModLow12 = 1 << (0 + OperandFlags::Count),
    ExprModHigh12 = 1 << (1 + OperandFlags::Count),
    ShiftLeft = 1 << (2 + OperandFlags::Count),
    Conditional = 1 << (3 + OperandFlags::Count)
};

enum AArch64Conditions : int64_t {
    Eq = 0b0000,
    Ne = 0b0001,
    Cs = 0b0010,
    Hs = Cs,    
    Cc = 0b0011,
    Lo = Cc,    
    Mi = 0b0100,
    Pl = 0b0101,
    Vs = 0b0110,
    Vc = 0b0111,
    Hi = 0b1000,
    Ls = 0b1001,
    Ge = 0b1010,
    Lt = 0b1011,
    Gt = 0b1100,
    Le = 0b1101,
    Al = 0b1110,
    Nv = 0b1111 
};

class AArch64InstructionInfo : public InstructionInfo {
public:
    AArch64InstructionInfo(RegisterInfo* registerInfo, Ref<Context> ctx);

    size_t registerToStackSlot(MIR::Block* block, size_t pos, MIR::Register* reg, MIR::StackSlot stackSlot) override;
    size_t stackSlotToRegister(MIR::Block* block, size_t pos, MIR::Register* reg, MIR::StackSlot stackSlot) override;
    size_t immediateToStackSlot(MIR::Block* block, size_t pos, MIR::ImmediateInt* immediate, MIR::StackSlot stackSlot) override;
    size_t move(MIR::Block* block, size_t pos, MIR::Operand* source, MIR::Operand* destination, size_t size, bool flt) override;
    
    size_t stackSlotAddress(MIR::Block* block, size_t pos, MIR::StackSlot stackSlot, MIR::Register* destination);

    size_t registerMemoryOp(MIR::Block* block, size_t pos, Opcode opcode, MIR::Register* op, MIR::Register* base, int64_t offset, Indexing indexing = Indexing::None, MIR::Symbol* symbol = nullptr);
    size_t registerMemoryOp(MIR::Block* block, size_t pos, Opcode opcode, MIR::Register* op, MIR::Register* base, MIR::Register* offset, Indexing indexing = Indexing::None, MIR::Symbol* symbol = nullptr);

    size_t registersMemoryOp(MIR::Block* block, size_t pos, Opcode opcode, std::initializer_list<MIR::Register*> ops, MIR::Register* base, int64_t offset, Indexing indexing = Indexing::None, MIR::Symbol* symbol = nullptr);
    size_t registersMemoryOp(MIR::Block* block, size_t pos, Opcode opcode, std::initializer_list<MIR::Register*> ops, MIR::Register* base, MIR::Register* offset, Indexing indexing = Indexing::None, MIR::Symbol* symbol = nullptr);

    MIR::Operand* getImmediate(MIR::Block* block, MIR::ImmediateInt* imm);
    MIR::Operand* getImmediate(MIR::Block* block, size_t& pos, MIR::ImmediateInt* imm);

    size_t getSymbolValue(MIR::Block* block, size_t pos, MIR::Symbol* symbol, MIR::Register* dst);
    size_t getSymbolAddress(MIR::Block* block, size_t pos, MIR::Symbol* symbol, MIR::Register* dst);
};

}