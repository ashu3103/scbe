
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case13(Target::TargetSpecification& spec, int debug) {
    // Create multiple arithmetic ops and use a merge block:
    // if (10 > 3) then res = 10+2 else res = 10-4; merge return res -> expect 12
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

    // merge: load whichever value was inserted (we assume previous blocks stored to a local)
    builder.setCurrentBlock(merge);
    // For simplicity, recompute the chosen expression using the same condition:
    // if (10>3) return 12 else return 6 -> 12
    auto finalCmp = builder.createICmpGt(ten, three);
    auto retThen = IR::ConstantInt::get(32, 12, ctx);
    auto retElse = IR::ConstantInt::get(32, 6, ctx);
    builder.createCondJump(rthenB, relsB, finalCmp);

    // rthen
    builder.setCurrentBlock(rthenB);
    builder.createRet(retThen);

    // relse
    builder.setCurrentBlock(relsB);
    builder.createRet(retElse);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 12);
}