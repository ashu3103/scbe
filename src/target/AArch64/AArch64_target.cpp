#include "target/AArch64/AArch64_target.hpp"
#include "target/AArch64/AArch64_target_machine.hpp"

namespace scbe::Target::AArch64 {

Ref<TargetMachine> AArch64Target::getTargetMachine(TargetSpecification spec, Ref<Context> context) {
    return std::make_shared<AArch64TargetMachine>(spec, context);
}

}