
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case5(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case5");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto neg8 = IR::ConstantInt::get(32, static_cast<uint64_t>(static_cast<int32_t>(-8)), ctx);
    auto one = IR::ConstantInt::get(32, 1, ctx);
    auto ar = builder.createARightShift(neg8, one);
    builder.createRet(ar);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), -4);
}