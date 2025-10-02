#include "target/call_info.hpp"
#include "MIR/function.hpp"
#include "IR/function.hpp"

namespace scbe::Target {

void CallInfo::analyzeCallOperands(CallConvFunction function, MIR::CallLowering* call) {
    m_argAssigns.clear();
    m_argAssigns.resize(call->getTypes().size() - 1);
    function(*this, call->getTypes());
}

void CallInfo::analyzeFormalArgs(CallConvFunction function, MIR::Function* call) {
    m_argAssigns.clear();
    m_argAssigns.resize(call->getIRFunction()->getFunctionType()->getContainedTypes().size() - 1);
    function(*this, call->getIRFunction()->getFunctionType()->getContainedTypes());
}

}