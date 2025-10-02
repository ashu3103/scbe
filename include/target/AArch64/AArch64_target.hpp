#pragma once

#include "target/target.hpp"

namespace scbe::Target::AArch64 {

class AArch64Target : public Target {
public:
    AArch64Target() : Target("AArch64") {}

    Ref<TargetMachine> getTargetMachine(TargetSpecification spec, Ref<Context> context) override;
};

}