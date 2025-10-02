#include "target/asm_printer.hpp"
#include "IR/function.hpp"
#include "MIR/function.hpp"
#include "unit.hpp"

namespace scbe::Target {

bool AsmPrinter::run(MIR::Function* function) {
    m_current = function;
    print(function);
    return true;
}

}