
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case13(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case13");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto ten = IR::ConstantInt::get(32, 10, ctx);
    auto three = IR::ConstantInt::get(32, 3, ctx);

    auto thenB = main->insertBlock("then");
    auto elsB = main->insertBlock("else");
    auto merge = main->insertBlock("merge");
    auto rthenB = main->insertBlock("rthen");
    auto relsB = main->insertBlock("relse");

    auto cmp = builder.createICmpGt(ten, three);
    builder.createCondJump(thenB, elsB, cmp);

    builder.setCurrentBlock(thenB);
    auto plus = builder.createAdd(ten, IR::ConstantInt::get(32, 2, ctx));
    builder.createJump(merge);

    builder.setCurrentBlock(elsB);
    auto minus = builder.createSub(ten, IR::ConstantInt::get(32, 4, ctx));
    builder.createJump(merge);

    builder.setCurrentBlock(merge);
    auto finalCmp = builder.createICmpGt(ten, three);
    auto retThen = IR::ConstantInt::get(32, 12, ctx);
    auto retElse = IR::ConstantInt::get(32, 6, ctx);
    builder.createCondJump(rthenB, relsB, finalCmp);

    builder.setCurrentBlock(rthenB);
    builder.createRet(retThen);

    builder.setCurrentBlock(relsB);
    builder.createRet(retElse);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 12);
}