#pragma once

#include "type.hpp"

#include <algorithm>
#include <string>

namespace scbe {
class Context;
}

namespace scbe::IR {

class Instruction;

class Value {
public:
    enum class ValueKind {
        ConstantInt,
        ConstantFloat,
        ConstantString,
        ConstantStruct,
        ConstantArray,
        ConstantZero,
        Block,
        Function,
        Register,
        FunctionArgument,
        GlobalVariable
    };

    virtual ~Value() {}

    std::string getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }
    virtual Type* getType() const { return m_type; }
    ValueKind getKind() const { return m_kind; }

    void addUse(Instruction* use) { m_uses.push_back(use); }
    const std::vector<Instruction*>& getUses() const { return m_uses; }
    void removeFromUses(Instruction* use) { m_uses.erase(std::remove(m_uses.begin(), m_uses.end(), use), m_uses.end()); }

    bool isConstantInt() const { return m_kind == ValueKind::ConstantInt; }
    bool isConstantFloat() const { return m_kind == ValueKind::ConstantFloat; }
    bool isConstantString() const { return m_kind == ValueKind::ConstantString; }
    bool isConstantStruct() const { return m_kind == ValueKind::ConstantStruct; }
    bool isConstantArray() const { return m_kind == ValueKind::ConstantArray; }
    bool isBlock() const { return m_kind == ValueKind::Block; }
    bool isFunction() const { return m_kind == ValueKind::Function; }
    bool isRegister() const { return m_kind == ValueKind::Register; }
    bool isFunctionArgument() const { return m_kind == ValueKind::FunctionArgument; }
    bool isGlobalVariable() const { return m_kind == ValueKind::GlobalVariable; }
    bool isZeroInitializer() const { return m_kind == ValueKind::ConstantZero; }

    bool isConstant() const { return m_kind >= ValueKind::ConstantInt && m_kind <= ValueKind::ConstantArray; }

protected:
    Value(std::string name, Type* type, ValueKind kind) : m_name(std::move(name)), m_type(type), m_kind(kind) {}
    Value() = default;

protected:
    std::string m_name = "";
    Type* m_type = nullptr;
    ValueKind m_kind;
    std::vector<Instruction*> m_uses;

friend class Function;
};

class Constant : public Value {
public:
    static Constant* getZeroInitalizer(Type* type, Ref<Context> context);

protected:
    Constant(Type* type, ValueKind kind, const std::string& name = "") : Value(name, type, kind) {}

friend class scbe::Context;
};

class ConstantInt : public Constant {
public:
    static ConstantInt* get(uint8_t bits, int64_t value, Ref<Context> context);

    int64_t getValue() const { return m_value; }

protected:
    ConstantInt(IntegerType* type, int64_t value) : Constant(type, ValueKind::ConstantInt), m_value(value) {}

private:
    int64_t m_value = 0;

friend class scbe::Context;
};

class ConstantFloat : public Constant {
public:
    static ConstantFloat* get(uint8_t bits, double value, Ref<Context> context);

    double getValue() const { return m_value; }
    void setValue(double value) { m_value = value; }

protected:
    ConstantFloat(FloatType* type, double value) : Constant(type, ValueKind::ConstantFloat), m_value(value) {}

private:
    double m_value = 0.0;

friend class scbe::Context;
};

class ConstantString : public Constant {
public:
    static ConstantString* get(const std::string& value, Ref<Context> context);

    const std::string& getValue() const { return m_value; }
    void setValue(const std::string& value) { m_value = value; }

protected:
    ConstantString(Type* type, const std::string& value) : Constant(type, ValueKind::ConstantString), m_value(value) {}

private:
    std::string m_value = "";

friend class scbe::Context;
};

class ConstantStruct : public Constant {
public:
    static ConstantStruct* get(StructType* type, const std::vector<Constant*>& values, Ref<Context> context);

    const std::vector<Constant*>& getValues() const { return m_values; }

protected:
    ConstantStruct(StructType* type, const std::vector<Constant*>& values) : Constant(type, ValueKind::ConstantStruct), m_values(values) {}

private:
    std::vector<Constant*> m_values;

friend class scbe::Context;
};

class ConstantArray : public Constant {
public:
    static ConstantArray* get(ArrayType* type, const std::vector<Constant*>& values, Ref<Context> context);

    const std::vector<Constant*>& getValues() const { return m_values; }

protected:
    ConstantArray(ArrayType* type, const std::vector<Constant*>& values) : Constant(type, ValueKind::ConstantArray), m_values(values) {}

private:
    std::vector<Constant*> m_values;

friend class scbe::Context;
};

class FunctionArgument : public Value {
public:
    FunctionArgument(const std::string& name, Type* type, uint32_t slot) : Value(name, type, ValueKind::FunctionArgument), m_slot(slot) {}

    uint32_t getSlot() const { return m_slot; }

private:
    uint32_t m_slot = 0;
};

}