#include "IR/function.hpp"
#include "MIR/operand.hpp"
#include "unit.hpp"
#include "IR/global_value.hpp"

namespace scbe::IR {

GlobalVariable* GlobalVariable::get(Unit& unit, Type* type, Constant* value, Linkage linkage, const std::string& name) {
    std::unique_ptr<GlobalVariable> var = std::unique_ptr<GlobalVariable>(new GlobalVariable(type, value, linkage, name));
    GlobalVariable* ret = var.get();
    unit.addGlobal(std::move(var));
    return ret;
}

MIR::GlobalAddress* GlobalValue::getMachineGlobalAddress(Unit& unit) {
    if(!m_machineGlobalAddress) {
        auto ptr = std::unique_ptr<MIR::GlobalAddress>(new MIR::GlobalAddress(this));
        m_machineGlobalAddress = ptr.get();
        unit.addSymbol(std::move(ptr));
    }
    return m_machineGlobalAddress;
}

}