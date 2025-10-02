
#include "IR/value.hpp"
#include "cases.hpp"
#include "context.hpp"
#include "IR/function.hpp"
#include "IR/block.hpp"
#include "IR/builder.hpp"

using namespace scbe;

bool case8(Target::TargetSpecification& spec, int debug) {
    // GEP + load/store: allocate [3 x i32], store 123 at index 2, load and return it.
    Unit unit = createUnit("case8");
    auto ctx = unit.getContext();
    auto fnTy = ctx->makeFunctionType({}, ctx->getI32Type());
    auto main = unit.addFunction(std::move(std::make_unique<IR::Function>("main", fnTy, IR::Linkage::External)));

    IR::Builder builder(ctx);
    auto entry = main->insertBlock("entry");
    builder.setCurrentBlock(entry);

    // allocate an array (represented: allocate element type with GEP)
    auto arrayTy = ctx->makeArrayType(ctx->getI32Type(), 3);
    auto arrayPtr = builder.createAllocate(arrayTy, "arr"); // treat as pointer to first element
    // indices: 0 (base), 2
    std::vector<IR::Value*> indic = {
        IR::ConstantInt::get(32, 0, ctx),
        IR::ConstantInt::get(32, 2, ctx)
    };
    auto gep = builder.createGEP(arrayPtr->getType(), arrayPtr, indic);
    auto val = IR::ConstantInt::get(32, 123, ctx);
    builder.createStore(gep, val);

    // load back
    auto loaded = builder.createLoad(gep);
    builder.createRet(loaded);

    auto program = compileUnit(unit, spec, debug);
    if(!program) return false;
    return expectInteger<uint8_t>(executeProgram(program.value(), spec.getArch()), 123);
}