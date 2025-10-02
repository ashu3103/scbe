#pragma once

namespace scbe::ISel::DAG {

class Node {
public:
    enum class NodeKind {
        Root,
        ConstantInt,
        ConstantFloat,
        Register,
        FrameIndex,
        FunctionArgument,
        GlobalValue,
        Ret,
        Load,
        Store,
        Add,
        Sub,
        ICmpEq,
        ICmpNe,
        ICmpGt,
        ICmpGe,
        ICmpLt,
        ICmpLe,
        UCmpGt,
        UCmpGe,
        UCmpLt,
        UCmpLe,
        FCmpEq,
        FCmpNe,
        FCmpGt,
        FCmpGe,
        FCmpLt,
        FCmpLe,
        Jump,
        Phi,
        LoadConstant,
        LoadGlobal,
        GEP,
        Call,
        Zext,
        Sext,
        Trunc,
        Fptrunc,
        Fpext,
        Fptosi,
        Fptoui,
        Sitofp,
        Uitofp,
        Ptrtoint,
        Inttoptr,
        ShiftLeft,
        LShiftRight,
        AShiftRight,
        And,
        Or,
        IDiv,
        UDiv,
        FDiv,
        IRem,
        URem,
        IMul,
        UMul,
        FMul,
        Switch,
        MultiValue,
        Count
    };

    virtual ~Node() {}
    
    Node(NodeKind kind) : m_kind(kind) {}

    NodeKind getKind() const { return m_kind; }

    bool isCmp() const { return m_kind >= NodeKind::ICmpEq && m_kind <= NodeKind::FCmpLe; }

protected:
    NodeKind m_kind;
};

}