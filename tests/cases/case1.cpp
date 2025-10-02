#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;


bool case1(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case1");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder (ctx);

    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto var = builder.createAllocate(ctx->getI32Type(), "var");
    auto one = IR::ConstantInt::get(32, 1, ctx);
    auto two = IR::ConstantInt::get(32, 2, ctx);
    auto fifty = IR::ConstantInt::get(32, 50, ctx);
    builder.createStore(var, fifty);
    

    auto then = main->insertBlock("then");
    auto els = main->insertBlock("else");
    auto merge = main->insertBlock("merge");

    auto cmp = builder.createICmpEq(builder.createLoad(var), fifty);
    builder.createCondJump(then, els, cmp);

    builder.setCurrentBlock(then);
    builder.createStore(var, builder.createSub(fifty, one));
    builder.createJump(merge);

    builder.setCurrentBlock(els);
    builder.createStore(var, two);
    builder.createJump(merge);

    builder.setCurrentBlock(merge);
    builder.createRet(builder.createLoad(var));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 49);
}
