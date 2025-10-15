#pragma once

#include "function.hpp"

namespace scbe {
class Context;
}

namespace scbe::IR {

class NativeFunction : public Function {
public:
    enum Name {
        Memcpy
    };

    static std::unique_ptr<NativeFunction> get(Name name, Ref<Context> ctx);

    Name getNativeName() const { return m_nativeName; }

private:
    NativeFunction(const std::string& name, FunctionType* type, Name nativeName) : Function(name, type, Linkage::Internal), m_nativeName(nativeName) { m_isNative = true; }

private:
    Name m_nativeName;
};

}