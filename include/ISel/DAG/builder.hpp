#pragma once

namespace scbe::ISel::DAG {

class Root;
class Chain;
class FrameIndex;
class Register;
class Value;
class Node;

class Builder {
public:
    ~Builder();

    Root* getRoot() { return m_root; }
    void setRoot(Root* root) { m_root = root; }
    void insert(std::unique_ptr<Node> node);

    std::unique_ptr<Root> createRoot(const std::string& name);

    Node* createAdd(Node* lhs, Node* rhs, Register* reg);
    Node* createSub(Node* lhs, Node* rhs, Register* reg);
    Node* createICmpEq(Node* lhs, Node* rhs, Register* reg);
    Node* createICmpNe(Node* lhs, Node* rhs, Register* reg);
    Node* createICmpGt(Node* lhs, Node* rhs, Register* reg);
    Node* createICmpGe(Node* lhs, Node* rhs, Register* reg);
    Node* createICmpLt(Node* lhs, Node* rhs, Register* reg);
    Node* createICmpLe(Node* lhs, Node* rhs, Register* reg);
    Node* createUCmpGt(Node* lhs, Node* rhs, Register* reg);
    Node* createUCmpGe(Node* lhs, Node* rhs, Register* reg);
    Node* createUCmpLt(Node* lhs, Node* rhs, Register* reg);
    Node* createUCmpLe(Node* lhs, Node* rhs, Register* reg);
    Node* createFCmpEq(Node* lhs, Node* rhs, Register* reg);
    Node* createFCmpNe(Node* lhs, Node* rhs, Register* reg);
    Node* createFCmpGt(Node* lhs, Node* rhs, Register* reg);
    Node* createFCmpGe(Node* lhs, Node* rhs, Register* reg);
    Node* createFCmpLt(Node* lhs, Node* rhs, Register* reg);
    Node* createFCmpLe(Node* lhs, Node* rhs, Register* reg);
    Node* createLeftShift(Node* lhs, Node* rhs, Register* reg);
    Node* createLRightShift(Node* lhs, Node* rhs, Register* reg);
    Node* createARightShift(Node* lhs, Node* rhs, Register* reg);
    Node* createAnd(Node* lhs, Node* rhs, Register* reg);
    Node* createOr(Node* lhs, Node* rhs, Register* reg);
    Node* createPhi(Register* reg, std::vector<Node*> values);
    Node* createGEP(Register* reg, Node* ptr, std::vector<Node*> indices);
    Node* createZext(Register* result, Node* value, Type* toType);
    Node* createSext(Register* result, Node* value, Type* toType);
    Node* createTrunc(Register* result, Node* value, Type* toType);
    Node* createFptrunc(Register* result, Node* value, Type* toType);
    Node* createFpext(Register* result, Node* value, Type* toType);
    Node* createFptosi(Register* result, Node* value, Type* toType);
    Node* createFptoui(Register* result, Node* value, Type* toType);
    Node* createSitofp(Register* result, Node* value, Type* toType);
    Node* createUitofp(Register* result, Node* value, Type* toType);
    Node* createPtrtoint(Register* result, Node* value, Type* toType);
    Node* createInttoptr(Register* result, Node* value, Type* toType);
    Node* createIDiv(Node* lhs, Node* rhs, Register* reg);
    Node* createUDiv(Node* lhs, Node* rhs, Register* reg);
    Node* createFDiv(Node* lhs, Node* rhs, Register* reg);
    Node* createIRem(Node* lhs, Node* rhs, Register* reg);
    Node* createURem(Node* lhs, Node* rhs, Register* reg);
    Node* createIMul(Node* lhs, Node* rhs, Register* reg);
    Node* createUMul(Node* lhs, Node* rhs, Register* reg);
    Node* createFMul(Node* lhs, Node* rhs, Register* reg);

private:
    Root* m_root = nullptr;
};

}