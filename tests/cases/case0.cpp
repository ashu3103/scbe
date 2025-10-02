#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case0(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case0");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder (ctx);

    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    builder.createRet(builder.createAdd(IR::ConstantInt::get(32, 1, ctx), IR::ConstantInt::get(32, 2, ctx)));

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 3);
} 