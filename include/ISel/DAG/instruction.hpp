#pragma once

#include "IR/function.hpp"
#include "node.hpp"
#include "type.hpp"
#include "value.hpp"

namespace scbe::ISel::DAG {

class Instruction : public Node {
public:
    Instruction(NodeKind kind) : Node(kind) {}
    Instruction(NodeKind kind, Value* result) : Node(kind), m_result(result) {}
    Instruction(NodeKind kind, Value* result, Node* operand) : Node(kind), m_result(result) { m_operands.push_back(operand); }
    Instruction(NodeKind kind, Value* result, Node* lhs, Node* rhs) : Node(kind), m_result(result) { m_operands.push_back(lhs); m_operands.push_back(rhs); }

    const std::vector<Node*>& getOperands() const { return m_operands; }
    Value* getResult() const { return m_result; }

    void addOperand(Node* operand) { m_operands.push_back(operand); }

protected:
    std::vector<Node*> m_operands;
    Value* m_result = nullptr;
};

class Chain : public Instruction {
public:
    Chain(NodeKind kind) : Instruction(kind) {}
    Chain(NodeKind kind, Value* result) : Instruction(kind, result) {}

    void setNext(Chain* next) { m_next = next; }
    Chain* getNext() const { return m_next; }

protected:
    Chain* m_next = nullptr;
};

class Root : public Chain {
public:
    Root(const std::string& name) : Chain(NodeKind::Root), m_name(name) {}

    const std::string& getName() const { return m_name; }

public:
    std::vector<std::unique_ptr<Node>> m_nodes; // root is the owner of all nodes

private:
    std::string m_name;
};

class Call : public Chain {
public:
    Call(Value* result, Node* callee, bool isResultUsed) : Chain(NodeKind::Call, result), m_isResultUsed(isResultUsed) { addOperand(callee); }

    bool isResultUsed() const { return m_isResultUsed; }

private:
    bool m_isResultUsed = true;
};

class Cast : public Instruction {
public:
    Cast(NodeKind kind, Register* result, Node* value, Type* type) : Instruction(kind, result), m_type(type) { addOperand(value); }

    Type* getType() const { return m_type; }

private:
    Type* m_type;
};

}