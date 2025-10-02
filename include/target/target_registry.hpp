#pragma once

#include "target/target.hpp"
#include "target/target_specification.hpp"

namespace scbe::Target {

class TargetRegistry {
public:
    void registerTarget(Arch arch, Ref<Target> target) { m_targets.insert({arch, target}); }
    Ref<Target> getTarget(const TargetSpecification& spec) const { return m_targets.at(spec.getArch()); }

private:
    UMap<Arch, Ref<Target>> m_targets;
};

}