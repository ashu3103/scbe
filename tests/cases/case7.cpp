
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case7(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case7");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto five = IR::ConstantInt::get(32, 5, ctx);
    auto three = IR::ConstantInt::get(32, 3, ctx);
    auto eight = IR::ConstantInt::get(32, 8, ctx);

    auto andv = builder.createAnd(five, three);
    auto orv = builder.createOr(andv, eight);
    builder.createRet(orv);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 9);
}