#pragma once

#include "target/call_info.hpp"

namespace scbe::Target::AArch64 {

void CCAArch64AAPCS64(CallInfo& info, const std::vector<Type*>& types);

}