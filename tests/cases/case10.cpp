
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case10(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case10");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto i8Val = IR::ConstantInt::get(8, 200, ctx);
    auto zext = builder.createZext(i8Val, ctx->getI32Type());
    auto threeHundred = IR::ConstantInt::get(32, 300, ctx);
    auto trunced = builder.createTrunc(threeHundred, ctx->getI8Type());
    auto zext2 = builder.createZext(trunced, ctx->getI32Type());

    auto sum = builder.createAdd(zext, zext2);
    builder.createRet(sum);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 244);
}