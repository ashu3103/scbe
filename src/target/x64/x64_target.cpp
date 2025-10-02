#include "target/x64/x64_target.hpp"
#include "target/x64/x64_target_machine.hpp"

namespace scbe::Target::x64 {

Ref<TargetMachine> x64Target::getTargetMachine(TargetSpecification spec, Ref<Context> context) {
    return std::make_shared<x64TargetMachine>(spec, context);
}

}