
#include "IR/value.hpp"
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case2(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case2");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto var1 = builder.createAllocate(ctx->getI32Type(), "var1");
    builder.createStore(var1, IR::ConstantInt::get(32, 1, ctx));
    auto var2 = builder.createAllocate(ctx->getI32Type(), "var2");
    builder.createStore(var2, IR::ConstantInt::get(32, 2, ctx));

    auto add = builder.createAdd(builder.createLoad(var1), builder.createLoad(var2));
    auto mul = builder.createIMul(add, IR::ConstantInt::get(32, 2, ctx));
    builder.createRet(builder.createIDiv(mul, IR::ConstantInt::get(32, 2, ctx)));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 3);
}