#pragma once

#include "ISel/DAG/node.hpp"
#include "MIR/block.hpp"
#include "MIR/operand.hpp"
#include "data_layout.hpp"

#include <functional>
#include <unordered_map>

namespace scbe {
class Context;
}

namespace scbe::Target {
class InstructionInfo;
}

namespace scbe::Codegen {
class DagISelPass;
}

#define MATCHER_ARGS ISel::DAG::Node* node, DataLayout* layout
#define EMITTER_ARGS MIR::Block* block, DataLayout* layout, scbe::Target::InstructionInfo* instrInfo, ISel::DAG::Node* node, Codegen::DagISelPass* isel, Ref<Context> context
#define MATCHER_SIG [](MATCHER_ARGS)
#define EMITTER_SIG [](EMITTER_ARGS)

namespace scbe::ISel::DAG {

using MatcherFunc = std::function<bool(MATCHER_ARGS)>;
using EmitterFunc = std::function<MIR::Operand*(EMITTER_ARGS)>;

struct Pattern {
    MatcherFunc match;
    EmitterFunc emit;

    uint32_t m_baseCost;
    std::vector<size_t> m_coveredOperands; // operands that this pattern absorbs, for multi node patterns
    std::string m_name;
};

class PatternBuilder {
public:
    PatternBuilder& forOpcode(Node::NodeKind opcode) {
        m_opcodes.clear();
        m_opcodes.push_back(opcode);
        return *this;
    }

    PatternBuilder& forOpcodes(const std::initializer_list<Node::NodeKind>& opcodes) {
        m_opcodes.clear();
        m_opcodes.insert(m_opcodes.end(), opcodes.begin(), opcodes.end());
        return *this;
    }

    PatternBuilder& match(MatcherFunc matcher) {
        m_currentMatcher = matcher;
        return *this;
    }

    PatternBuilder& emit(EmitterFunc emitter) {
        m_currentEmitter = emitter;
        Pattern p = {m_currentMatcher, m_currentEmitter};
        for(auto op : m_opcodes)
            m_patterns[op].push_back(p);
        return *this;
    }

    PatternBuilder& withBaseCost(uint32_t cost) {
        for(auto op : m_opcodes)
            m_patterns[op].back().m_baseCost = cost;
        return *this;
    }

    PatternBuilder& withCoveredOperands(const std::vector<size_t>& coveredOperands) {
        for(auto op : m_opcodes)
            m_patterns[op].back().m_coveredOperands = coveredOperands;
        return *this;
    }

    PatternBuilder& withName(const std::string& name) {
        for(auto op : m_opcodes)
            m_patterns[op].back().m_name = name;
        return *this;
    }

    std::unordered_map<Node::NodeKind, std::vector<Pattern>> build() { return m_patterns; }

private:
    MatcherFunc m_currentMatcher = nullptr;
    EmitterFunc m_currentEmitter = nullptr;
    std::vector<Node::NodeKind> m_opcodes;

    std::unordered_map<Node::NodeKind, std::vector<Pattern>> m_patterns;
};

}