#pragma once

#include "unit.hpp"
#include "IR/function.hpp"
#include "IR/global_value.hpp"
#include "target/target_specification.hpp"

#include <optional>
#include <stdexcept>

scbe::Unit createUnit(const std::string& name);

std::string archString(scbe::Target::Arch arch);

std::optional<std::string> compileUnit(scbe::Unit& unit, scbe::Target::TargetSpecification spec, int debug);

uint8_t executeProgram(std::string program, scbe::Target::Arch arch, std::vector<std::string> args = {});

template<typename T>
inline bool expectInteger(T actual, T expected) {
    if(expected != actual)
        throw std::runtime_error("Expected " + std::to_string(expected) + " but got " + std::to_string(actual));
    return true;
}


bool case0(scbe::Target::TargetSpecification& spec, int debug);
bool case1(scbe::Target::TargetSpecification& spec, int debug);
bool case2(scbe::Target::TargetSpecification& spec, int debug);
bool case3(scbe::Target::TargetSpecification& spec, int debug);
bool case4(scbe::Target::TargetSpecification& spec, int debug);
bool case5(scbe::Target::TargetSpecification& spec, int debug);
bool case6(scbe::Target::TargetSpecification& spec, int debug);
bool case7(scbe::Target::TargetSpecification& spec, int debug);
bool case8(scbe::Target::TargetSpecification& spec, int debug);
bool case9(scbe::Target::TargetSpecification& spec, int debug);
bool case10(scbe::Target::TargetSpecification& spec, int debug);
bool case11(scbe::Target::TargetSpecification& spec, int debug);
bool case12(scbe::Target::TargetSpecification& spec, int debug);
bool case13(scbe::Target::TargetSpecification& spec, int debug);
bool case14(scbe::Target::TargetSpecification& spec, int debug);