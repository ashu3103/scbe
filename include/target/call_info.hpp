#pragma once

#include <cstdint>
#include <functional>
#include <vector>

namespace scbe {
class DataLayout;
}

namespace scbe::MIR {
class Function;
class CallLowering;
}

namespace scbe::Target {

class CallInfo;
class RegisterInfo;

using CallConvFunction = std::function<void(CallInfo& info, const std::vector<Type*>& types)>;

class ArgAssign {
public:
    enum class Kind {
        Register,
        Stack
    };

    ArgAssign(Kind kind) : m_kind(kind) {}
    virtual ~ArgAssign() {}

    Kind getKind() const { return m_kind; }

protected:
    Kind m_kind;
};

class RegisterAssign : public ArgAssign {
public:
    RegisterAssign(uint32_t reg) : ArgAssign(Kind::Register), m_register(reg) {}

    uint32_t getRegister() const { return m_register; }

private:
    uint32_t m_register;
};

class StackAssign : public ArgAssign {
public:
    StackAssign() : ArgAssign(Kind::Stack) {}
};

class CallInfo {
public:
    CallInfo(RegisterInfo* registerInfo, DataLayout* dataLayout) : m_registerInfo(registerInfo), m_dataLayout(dataLayout) {}

    void setArgAssign(uint32_t index, Ref<ArgAssign> assign) { m_argAssigns[index] = assign; }

    void analyzeCallOperands(CallConvFunction function, MIR::CallLowering* call);
    void analyzeFormalArgs(CallConvFunction function, MIR::Function* call);
    const std::vector<Ref<ArgAssign>>& getArgAssigns() const { return m_argAssigns; }
    const std::vector<Ref<ArgAssign>>& getRetAssigns() const { return m_retAssigns; }

    void addRetAssign(Ref<ArgAssign> assign) { m_retAssigns.push_back(assign); }

    RegisterInfo* getRegisterInfo() const { return m_registerInfo; }
    DataLayout* getDataLayout() const { return m_dataLayout; }

protected:
    std::vector<Ref<ArgAssign>> m_argAssigns;
    std::vector<Ref<ArgAssign>> m_retAssigns;

    RegisterInfo* m_registerInfo;
    DataLayout* m_dataLayout;
};

}