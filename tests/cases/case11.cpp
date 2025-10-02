
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case11(Target::TargetSpecification& spec, int debug) {
    // bitcast + ptrtoint + inttoptr + global string load: return first char of string "Z" (90)
    Unit unit = createUnit("case11");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    auto gstr = unit.createGlobalString("Z_rest", "gstr"); // pointer to i8
    // convert pointer -> int64
    auto ptrToInt = builder.createPtrtoint(gstr, ctx->getI64Type());
    // convert back to pointer type (assume i8*)
    auto intToPtr = builder.createInttoptr(ptrToInt, ctx->makePointerType(ctx->getI8Type()));
    // load first byte
    auto loaded = builder.createLoad(intToPtr);
    // bitcast loaded i8 -> i32 for return (use zext)
    auto retv = builder.createZext(loaded, ctx->getI32Type());
    builder.createRet(retv);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    // ASCII 'Z' == 90
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 90);
}