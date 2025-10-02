#pragma once

#include "target/target_machine.hpp"
#include "target/target_specification.hpp"
#include <string>

namespace scbe::Target {

class Target {
public:
    Target(const std::string& name) : m_name(name) {}

    virtual Ref<TargetMachine> getTargetMachine(TargetSpecification spec, Ref<Context> context) = 0;

    const std::string& getName() const { return m_name; }

protected:
    std::string m_name;
};

}