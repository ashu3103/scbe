
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case12(Target::TargetSpecification& spec, int debug) {
    // multiple branches with ICmpNe: if (5 != 5) -> else branch returns 77
    Unit unit = createUnit("case12");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto fiveA = IR::ConstantInt::get(32, 5, ctx);
    auto fiveB = IR::ConstantInt::get(32, 5, ctx);

    auto thenB = main->insertBlock("then");
    auto elsB = main->insertBlock("else");

    auto cmp = builder.createICmpNe(fiveA, fiveB);
    builder.createCondJump(thenB, elsB, cmp);

    builder.setCurrentBlock(thenB);
    builder.createRet(IR::ConstantInt::get(32, 0, ctx));

    builder.setCurrentBlock(elsB);
    builder.createRet(IR::ConstantInt::get(32, 77, ctx));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 77);
}