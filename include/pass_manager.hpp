#pragma once

#include "pass.hpp"

#include <vector>

namespace scbe {

class PassManager {
public:
    void addPass(Ref<Pass> pass) { m_passes.push_back(std::move(pass)); }

    void run(Unit& unit);

protected:
    std::vector<Ref<Pass>> m_passes;
};

}