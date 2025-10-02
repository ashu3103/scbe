
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case3(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case3");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);

    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto negOne = IR::ConstantInt::get(32, -1, ctx);
    auto one = IR::ConstantInt::get(32, 1, ctx);

    auto thenB = main->insertBlock("then");
    auto elsB  = main->insertBlock("else");

    auto cmp = builder.createICmpLt(negOne, one);
    builder.createCondJump(thenB, elsB, cmp);

    builder.setCurrentBlock(thenB);
    builder.createRet(one);

    builder.setCurrentBlock(elsB);
    builder.createRet(IR::ConstantInt::get(32, 0, ctx));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 1);
}