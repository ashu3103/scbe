
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case6(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case6");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto three = IR::ConstantInt::get(32, 3, ctx);
    auto threeShift = IR::ConstantInt::get(32, 3, ctx);
    auto two = IR::ConstantInt::get(32, 2, ctx);

    auto left = builder.createLeftShift(three, threeShift);
    auto lrs = builder.createLRightShift(left, two);
    builder.createRet(lrs);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 6);
}