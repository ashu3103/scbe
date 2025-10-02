
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case11(Target::TargetSpecification& spec, int debug) {
    Unit unit = createUnit("case11");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto gstr = unit.createGlobalString("Z_rest", "gstr");
    auto ptrToInt = builder.createPtrtoint(gstr, ctx->getI64Type());
    auto intToPtr = builder.createInttoptr(ptrToInt, ctx->makePointerType(ctx->getI8Type()));
    auto loaded = builder.createLoad(intToPtr);
    auto retv = builder.createZext(loaded, ctx->getI32Type());
    builder.createRet(retv);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 90);
}