#pragma once

#include "codegen/object_emitter.hpp"

namespace scbe::Codegen {

class ELFObjectEmitter : public ObjectEmitter {
public:
    ELFObjectEmitter(std::ofstream& output, Ref<InstructionEncoder> encoder, Target::InstructionInfo* info) : ObjectEmitter(output, encoder, info) {}

    void emitObjectFile(Unit& unit) override;
};

}