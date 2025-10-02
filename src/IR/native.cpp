#include "IR/native.hpp"
#include "context.hpp"

namespace scbe::IR {

Ref<NativeFunction> NativeFunction::get(Name name, Ref<Context> ctx) {
    switch (name) {
        case Name::Memcpy:
            static Ref<NativeFunction> memcpy =
                std::shared_ptr<NativeFunction>(new NativeFunction(
                    "builtin.memcpy",
                    ctx->makeFunctionType({ctx->makePointerType(ctx->getI8Type()), ctx->makePointerType(ctx->getI8Type()), ctx->getI64Type()}, ctx->getVoidType()),
                    Name::Memcpy
                ));
            return memcpy;
            break;
    }
    return nullptr;
}

}