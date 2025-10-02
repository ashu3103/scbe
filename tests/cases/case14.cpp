#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case14(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case0");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({ctx->getI32Type()}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder (ctx);

    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto case0 = main->insertBlock("case0");
    auto case1 = main->insertBlock("case1");
    auto case2 = main->insertBlock("case2");

    std::vector<std::pair<IR::ConstantInt*, IR::Block*>> cases = {
        {ctx->getConstantInt(32, 1), case0},
        {ctx->getConstantInt(32, 2), case1},
        {ctx->getConstantInt(32, 3), case2}    
    };
    builder.createSwitch(main->getArguments().at(0).get(), case0, cases);

    builder.setCurrentBlock(case0);
    builder.createRet(ctx->getConstantInt(32, 0));
    builder.setCurrentBlock(case1);
    builder.createRet(ctx->getConstantInt(32, 1));
    builder.setCurrentBlock(case2);
    builder.createRet(ctx->getConstantInt(32, 2));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch(), {"a"}), 1);
} 