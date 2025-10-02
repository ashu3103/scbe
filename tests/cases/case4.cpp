
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case4(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case4");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    uint64_t big = 4000000000ULL;
    auto bigConst = IR::ConstantInt::get(32, big, ctx);
    auto small = IR::ConstantInt::get(32, 100, ctx);

    auto thenB = main->insertBlock("then");
    auto elsB  = main->insertBlock("else");

    auto cmp = builder.createUCmpGt(bigConst, small);
    builder.createCondJump(thenB, elsB, cmp);

    builder.setCurrentBlock(thenB);
    builder.createRet(IR::ConstantInt::get(32, 1, ctx));

    builder.setCurrentBlock(elsB);
    builder.createRet(IR::ConstantInt::get(32, 0, ctx));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 1);
}