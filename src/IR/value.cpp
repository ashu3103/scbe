#include "context.hpp"
#include "type_alias.hpp"
#include "IR/value.hpp"

#include <cstdint>

namespace scbe::IR {

Constant* Constant::getZeroInitalizer(Type* type, Ref<Context> context) {
    return context->getZeroInitalizer(type);
}

ConstantInt* ConstantInt::get(uint8_t bits, int64_t value, Ref<Context> context) {
    return context->getConstantInt(bits, value);
}

ConstantFloat* ConstantFloat::get(uint8_t bits, double value, Ref<Context> context) {
    return context->getConstantFloat(bits, value);
}

ConstantString* ConstantString::get(const std::string& value, Ref<Context> context) {
    return context->getConstantString(value);
}

ConstantStruct* ConstantStruct::get(StructType* type, const std::vector<Constant*>& values, Ref<Context> context) {
    return context->getConstantStruct(type, values);
}

ConstantArray* ConstantArray::get(ArrayType* type, const std::vector<Constant*>& values, Ref<Context> context) {
    return context->getConstantArray(type, values);
}

}