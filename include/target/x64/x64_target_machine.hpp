#pragma once

#include "target/instruction_info.hpp"
#include "target/register_info.hpp"
#include "target/target_machine.hpp"
#include <functional>
#include <initializer_list>

namespace scbe::Target::x64 {

class x64TargetMachine : public TargetMachine {
public:
    x64TargetMachine(TargetSpecification spec, Ref<Context> context) : TargetMachine(spec, context) {}

    void addPassesForCodeGeneration(Ref<PassManager> passManager, std::ofstream& output, FileType type, OptimizationLevel level) override;
    void addPassesForCodeGeneration(Ref<PassManager> passManager, std::initializer_list<std::reference_wrapper<std::ofstream>> files, std::initializer_list<FileType> type, OptimizationLevel level) override;
    DataLayout* getDataLayout() override;
    RegisterInfo* getRegisterInfo() override;
    InstructionInfo* getInstructionInfo() override;
};

}