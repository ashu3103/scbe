#pragma once

#include "codegen/fixup.hpp"
#include "codegen/instruction_encoder.hpp"
#include "pass.hpp"
#include "target/instruction_info.hpp"
#include "target/register_info.hpp"

namespace scbe::Codegen {

struct DataEntry {
    size_t m_loc;
    MIR::GlobalAddress* m_globalAddress;
};

class ObjectEmitter : public MachineFunctionPass {
public: 
    ObjectEmitter(std::ofstream& output, Ref<InstructionEncoder> encoder, Target::InstructionInfo* info) : m_encoder(encoder), m_instructionInfo(info), m_registerInfo(info->getRegisterInfo()), m_output(output) {}

    bool run(MIR::Function* function) override;
    void end(Unit& unit) override;
    void init(Unit& unit) override;

    void encodeConstant(IR::Constant* constant, DataLayout* layout);

    virtual void emitObjectFile(Unit& unit) = 0;

protected:
    Ref<InstructionEncoder> m_encoder = nullptr;

    std::vector<uint8_t> m_codeBytes;
    std::vector<uint8_t> m_dataBytes;

    std::vector<Fixup> m_fixups;
    UMap<std::string, size_t> m_codeLocTable;
    UMap<std::string, DataEntry> m_dataLocTable;

    Target::InstructionInfo* m_instructionInfo;
    Target::RegisterInfo* m_registerInfo;
    std::ofstream& m_output;
};

}