
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case9(Target::TargetSpecification& spec, int debug) {
    // call: create a callee function that returns 7, main calls it and returns callee()+1 -> 8
    Unit unit = createUnit("case9");
    auto ctx = unit.getContext();
    auto fnTyCallee = ctx->makeFunctionType({}, ctx->getI32Type());
    auto callee = unit.addFunction(std::move(std::make_unique<IR::Function>("callee", fnTyCallee, IR::Linkage::External)));

    // define callee body
    {
        IR::Builder b(ctx);
        auto entry = callee->insertBlock("entry");
        b.setCurrentBlock(entry);
        b.createRet(IR::ConstantInt::get(32, 7, ctx));
    }

    // main
    auto fnTyMain = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTyMain, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto callv = builder.createCall(callee);
    auto one = IR::ConstantInt::get(32, 1, ctx);
    auto add = builder.createAdd(callv, one);
    builder.createRet(add);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 8);
}