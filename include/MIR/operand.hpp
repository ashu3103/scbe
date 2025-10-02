#pragma once

#include "IR/global_value.hpp"
#include "type_alias.hpp"
#include "cast.hpp"

#include <cstdint>

namespace scbe::Target {
class RegisterInfo;
}

namespace scbe::MIR {

class Function;
class Block;

class Operand {
public:
    enum class Kind {
        Register,
        ImmediateInt,
        FrameIndex,
        ConstantIndex,
        GlobalAddress,
        Block,
        ExternalSymbol,
        MultiValue,
        Count
    };

    Operand(Kind kind) : m_kind(kind) {}

    Kind getKind() const { return m_kind; }
    bool hasFlag(int64_t flag) const { return (m_flags & flag) != 0; }

    void addFlag(int64_t flag) { m_flags |= flag; }
    void setFlags(int64_t flags) { m_flags = flags; }
    void clearFlags() { m_flags = 0; }
    int64_t getFlags() const { return m_flags; }

    bool isRegister() const { return m_kind == Kind::Register; }
    bool isImmediateInt() const { return m_kind == Kind::ImmediateInt; }
    bool isFrameIndex() const { return m_kind == Kind::FrameIndex; }
    bool isConstantIndex() const { return m_kind == Kind::ConstantIndex; }
    bool isGlobalAddress() const { return m_kind == Kind::GlobalAddress; }
    bool isBlock() const { return m_kind == Kind::Block; }
    bool isExternalSymbol() const { return m_kind == Kind::ExternalSymbol; }
    bool isMultiValue() const { return m_kind == Kind::MultiValue; }

    virtual bool equals(MIR::Operand* other, bool ignoreFlags) const { return ignoreFlags || m_flags == other->m_flags; }

protected:
    Kind m_kind;

    int64_t m_flags = 0;
};

class Register: public Operand {
public:
    uint32_t getId() const { return m_id; }    

    bool equals(MIR::Operand* other, bool ignoreFlags) const override { return other->isRegister() && cast<Register>(other)->getId() == m_id && Operand::equals(other, ignoreFlags); }

protected:
    Register(uint32_t id) : Operand(Kind::Register), m_id(id) {}

private:
    uint32_t m_id;

friend class Target::RegisterInfo;
};

class ImmediateInt: public Operand {
public:
    enum Size : size_t {
        imm8 = 1,
        imm16 = 2,
        imm32 = 4,
        imm64 = 8
    };
    ImmediateInt(int64_t value, Size size) : Operand(Kind::ImmediateInt), m_value(value), m_size(size) {}

    int64_t getValue() const { return m_value; }
    Size getSize() const { return m_size; }

    bool equals(MIR::Operand* other, bool ignoreFlags) const override { return other->isImmediateInt() && cast<ImmediateInt>(other)->getValue() == m_value && cast<ImmediateInt>(other)->getSize() == m_size && Operand::equals(other, ignoreFlags); }

private:
    int64_t m_value;
    Size m_size;
};

class FrameIndex: public Operand {
public:
    FrameIndex(uint32_t index) : Operand(Kind::FrameIndex), m_index(index) {}

    uint32_t getIndex() const { return m_index; }

    bool equals(MIR::Operand* other, bool ignoreFlags) const override { return other->isFrameIndex() && cast<FrameIndex>(other)->getIndex() == m_index && Operand::equals(other, ignoreFlags); }

private:
    uint32_t m_index;
};

class Symbol : public Operand {
public:
    Symbol(std::string name, Operand::Kind kind) : Operand(kind), m_name(name) {}

    const std::string& getName() const { return m_name; }

    bool equals(MIR::Operand* other, bool ignoreFlags) const override { return other->isRegister() && cast<Symbol>(other)->getName() == m_name && Operand::equals(other, ignoreFlags); }

protected:
    std::string m_name;
};

class ConstantIndex : public Symbol {
public:
    ConstantIndex(const std::string& name, size_t index) : Symbol(name, Kind::ConstantIndex), m_index(index) {}

    size_t getIndex() const { return m_index; }

    bool equals(MIR::Operand* other, bool ignoreFlags) const override { return other->isConstantIndex() && cast<ConstantIndex>(other)->getIndex() == m_index && Operand::equals(other, ignoreFlags); }

private:
    size_t m_index;
};

class GlobalAddress : public Symbol {
public: 
    GlobalAddress(GlobalAddress& other) : Symbol(other.getName(), Kind::GlobalAddress), m_value(other.m_value) {}

    IR::GlobalValue* getValue() const { return m_value; }    

protected:
    GlobalAddress(IR::GlobalValue* value) : Symbol(value->getName(), Kind::GlobalAddress), m_value(value) {}

private:
    IR::GlobalValue* m_value;
    
friend class IR::GlobalValue;
friend class Function;
friend class Unit;
};

class ExternalSymbol : public Symbol {
protected:
    ExternalSymbol(const std::string& symbol) : Symbol(symbol, Kind::ExternalSymbol) {}

friend class scbe::Unit;
friend class Function;
};

// Used solely during instruction selection to return multiple values. do not use in final instructions
class MultiValue : public Operand {
public:
    MultiValue() : Operand(Kind::MultiValue) {}

    void addValue(MIR::Operand* value) { m_values.push_back(value); }
    const std::vector<MIR::Operand*>& getValues() const { return m_values; }    
private:
    std::vector<MIR::Operand*> m_values;
};
    
}