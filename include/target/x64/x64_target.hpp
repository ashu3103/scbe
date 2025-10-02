#pragma once

#include "target/target.hpp"

namespace scbe::Target::x64 {

class x64Target : public Target {
public:
    x64Target() : Target("x64") {}

    Ref<TargetMachine> getTargetMachine(TargetSpecification spec, Ref<Context> context) override;
};

}