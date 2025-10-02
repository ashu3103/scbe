#pragma once

#include "target/call_info.hpp"

namespace scbe::Target::x64 {

void CCx64SysV(CallInfo& info, const std::vector<Type*>& types);
void CCx64Win64(CallInfo& info, const std::vector<Type*>& types);

}