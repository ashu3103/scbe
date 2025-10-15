#include "target/x64/x64_calling_conventions.hpp"
#include "target/call_info.hpp"
#include "target/x64/x64_register_info.hpp"

namespace scbe::Target::x64 {

struct ArgAssignmentState {
    ArgAssignmentState(const std::vector<RegisterId>& gprs, const std::vector<RegisterId>& fprs) : GPRs(gprs), FPRs(fprs) {}
    
    size_t m_usedGPR = 0;
    size_t m_usedFPR = 0;

    std::vector<RegisterId> GPRs;
    std::vector<RegisterId> FPRs;

    bool hasGPR() const { return m_usedGPR < GPRs.size(); }
    bool hasFPR() const { return m_usedFPR < FPRs.size(); }

    RegisterId nextGPR() { return GPRs[m_usedGPR++]; }
    RegisterId nextFPR() { return FPRs[m_usedFPR++]; }
};

void CCx64SysV(CallInfo& info, const std::vector<Type*>& types) {
    ArgAssignmentState state(
        {RDI, RSI, RDX, RCX, R8, R9},
        {XMM0, XMM1, XMM2, XMM3, XMM4, XMM5}
    );
    ArgAssignmentState retState(
        {RAX, RDX},
        {XMM0, XMM1}
    );

    for (size_t i = 0; i < types.size() - 1; ++i) {
        Type* type = types[i + 1];

        bool isFloat = type->isFltType();
        size_t size = info.getDataLayout()->getSize(type);
        size_t align = info.getDataLayout()->getAlignment(type);

        if ((isFloat && state.hasFPR()) || (!isFloat && state.hasGPR())) {
            RegisterId baseReg = isFloat ? state.nextFPR() : state.nextGPR();
            size_t registerSize = info.getRegisterInfo()->getRegisterClass(
                info.getRegisterInfo()->getRegisterDesc(baseReg).getRegClass()
            ).getSize();
            uint32_t rId = isFloat ? baseReg : size == registerSize ? baseReg : info.getRegisterInfo()->getRegisterWithSize(baseReg, size).value();

            Ref<ArgAssign> assign = std::make_shared<RegisterAssign>(rId);
            info.setArgAssign(i, assign);
        } else {
            Ref<ArgAssign> assign = std::make_shared<StackAssign>();
            info.setArgAssign(i, assign);
        }
    }

    Type* retType = types[0];
    if(retType->isVoidType()) {
        // info.addRetAssign(nullptr);
        return;
    }
    else if(retType->isFltType()) {
        info.addRetAssign(std::make_shared<RegisterAssign>(XMM0));
        return;
    }
    else if(retType->isStructType()) {
        for(auto& field : retType->getContainedTypes()) {
            if(field->isIntType() && retState.hasGPR()) {
                uint32_t retId = info.getRegisterInfo()->getRegisterWithSize(retState.nextGPR(), info.getDataLayout()->getSize(field)).value();
                info.addRetAssign(std::make_shared<RegisterAssign>(retId));
            }
            else if(field->isFltType() && retState.hasFPR()) {
                info.addRetAssign(std::make_shared<RegisterAssign>(retState.nextFPR()));
            }
            else {
                throw std::runtime_error("Ran out of return registers");
            }
        }
        return;
    }
    uint32_t retId = info.getRegisterInfo()->getRegisterWithSize(RAX, info.getDataLayout()->getSize(retType)).value();
    info.addRetAssign(std::make_shared<RegisterAssign>(retId));
}

void CCx64Win64(CallInfo& info, const std::vector<Type*>& types) {
    ArgAssignmentState state(
        {RCX, RDX, R8, R9},
        {XMM0, XMM1, XMM2, XMM3}
    );

    Type* retType = types[0];
    if(retType->isVoidType()) {
        // info.addRetAssign(nullptr);
    } else if(retType->isFltType()) {
        info.addRetAssign(std::make_shared<RegisterAssign>(XMM0));
    } else if (retType->isStructType()) {
        size_t size = info.getDataLayout()->getSize(retType);
        if (size == 1 || size == 2 || size == 4 || size == 8) {
            info.addRetAssign(std::make_shared<RegisterAssign>(RAX));
        } else if (size == 16) {
            info.addRetAssign(std::make_shared<RegisterAssign>(RAX));
            info.addRetAssign(std::make_shared<RegisterAssign>(RDX));
        }
    } else {
        uint32_t retId = info.getRegisterInfo()->getRegisterWithSize(RAX, info.getDataLayout()->getSize(retType)).value();
        info.addRetAssign(std::make_shared<RegisterAssign>(retId));
    }

    for (size_t i = 0; i < types.size() - 1; ++i) {
        Type* type = types[i + 1];
        bool isFloat = type->isFltType();
        size_t size = info.getDataLayout()->getSize(type);
        size_t align = info.getDataLayout()->getAlignment(type);

        if ((isFloat && state.hasFPR()) || (!isFloat && state.hasGPR())) {
            RegisterId baseReg = isFloat ? state.nextFPR() : state.nextGPR();
            size_t registerSize = info.getRegisterInfo()->getRegisterClass(
                info.getRegisterInfo()->getRegisterDesc(baseReg).getRegClass()
            ).getSize();
            uint32_t rId = (!isFloat && size != registerSize)
                           ? info.getRegisterInfo()->getRegisterWithSize(baseReg, size).value()
                           : baseReg;

            Ref<ArgAssign> assign = std::make_shared<RegisterAssign>(rId);
            info.setArgAssign(i, assign);
        } else {
            Ref<ArgAssign> assign = std::make_shared<StackAssign>();
            info.setArgAssign(i, assign);
        }
    }
}

}